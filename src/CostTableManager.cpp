#include "CostTableManager.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cmath>

namespace {
    // Extract device name from one DeviceUnit: pick the first token not equal to "d"
    static std::string ExtractName(DeviceUnit& du) {
        if (du.GetDeviceNum() <= 0) return {};
        auto v = du.GetDeviceOutputVector();
        for (const auto& t : v) {
            if (t != "d") return t;
        }
        return {};
    }
} // namespace

// Calculate all three cost terms: CC, RC, and Separation
std::vector<double> CostTableManager::CalculateCost()
{
    this->CCCost = this->CalculateCCCost();
    this->RCCost = this->CalculateRCCost();
    this->SeperationCost = this->CalculateSeperationCost();
    return { this->CCCost, this->RCCost, this->SeperationCost };
}

// -----------------------------------------------------------------------------
// 1. CC Cost
// -----------------------------------------------------------------------------
double CostTableManager::CalculateCCCost() {
    using Acc = std::pair<long long, long long>; // {sum_x, count}
    std::unordered_map<std::string, Acc> acc;

    const int R = GetRowSize();
    const int C = GetColSize();

    for (int r = 0; r < R; ++r) {
        std::vector<std::string> rowTokens;
        rowTokens.reserve(128);

        for (int c = 0; c < C; ++c) {
            DeviceUnit& du = deviceUnitTable[r][c];
            auto v = du.GetDeviceOutputVector();
            rowTokens.insert(rowTokens.end(), v.begin(), v.end());
        }

        const int W = static_cast<int>(rowTokens.size());
        if (W == 0) continue;

        const bool even = (W % 2 == 0);
        const int  k = W / 2;
        const int  m = (W - 1) / 2;

        for (int i = 0; i < W; ++i) {
            const std::string& tok = rowTokens[i];
            if (tok == "d") continue;

            long long x;
            if (even) x = (i < k) ? (i - k) : (i - k + 1);
            else      x = i - m;

            auto& a = acc[tok];
            a.first += x;   // sum_x
            a.second += 1;  // count
        }
    }

    double sum = 0.0;
    int    n = 0;
    for (const auto& kv : acc) {
        const auto& a = kv.second;
        if (a.second == 0) continue;
        double mu = static_cast<double>(a.first) / static_cast<double>(a.second);
        sum += std::fabs(mu);
        ++n;
    }
    if (n == 0) return 0.0;
    return sum / static_cast<double>(n);
}

// -----------------------------------------------------------------------------
// 2. RC Cost
// -----------------------------------------------------------------------------
double CostTableManager::CalculateRCCost() {
    using std::string;
    using std::unordered_map;
    using std::unordered_set;
    using std::vector;

    const int Rn = GetRowSize();
    const int Cn = GetColSize();

    // --- R: for each device name, compute the population standard deviation
    //     of its average distance from the row center line ---
    unordered_map<string, long long> r_cnt_total;
    unordered_map<string, double>    r_sum_wdist;

    for (int r = 0; r < Rn; ++r) {
        const bool even = (Cn % 2 == 0);
        const int  k = Cn / 2;
        const int  m = (Cn - 1) / 2;

        for (int j = 0; j < Cn; ++j) {
            DeviceUnit& du = deviceUnitTable[r][j];
            string name = ExtractName(du);
            if (name.empty()) continue;

            double dist = even
                ? std::fabs((static_cast<double>(j) + 0.5) - static_cast<double>(k))
                : std::fabs(static_cast<double>(j - m));

            int cnt = du.GetDeviceNum();
            if (cnt <= 0) continue;

            r_sum_wdist[name] += dist * static_cast<double>(cnt);
            r_cnt_total[name] += static_cast<long long>(cnt);
        }
    }

    vector<double> r_per_name_cost;
    r_per_name_cost.reserve(r_sum_wdist.size());
    for (auto& kv : r_sum_wdist) {
        const string& name = kv.first;
        long long tot = r_cnt_total[name];
        if (tot <= 0) continue;
        r_per_name_cost.push_back(kv.second / static_cast<double>(tot));
    }

    double r = 0.0;
    if (!r_per_name_cost.empty()) {
        double s = 0.0, s2 = 0.0;
        for (double v : r_per_name_cost) { s += v; s2 += v * v; }
        double n = static_cast<double>(r_per_name_cost.size());
        double mean = s / n;
        double var = (s2 / n) - mean * mean;
        if (var < 0.0) var = 0.0;
        r = std::sqrt(var);
    }

    // --- C: from both sides toward center, increase "numerator" by 1
    //     only when a new letter appears for the first time ---
    unordered_map<string, double>    c_sum_weight;
    unordered_map<string, long long> c_cnt_total;

    auto process_side = [&](int row, int start, int end, int step) {
        unordered_set<string> seen;
        string prev;
        int numerator = 1;
        for (int j = start; j != end + step; j += step) {
            DeviceUnit& du = deviceUnitTable[row][j];
            string name = ExtractName(du);
            if (name.empty()) continue;

            if (!prev.empty() && name != prev && !seen.count(name)) {
                numerator += 1;
            }
            prev = name;
            seen.insert(name);

            int nfin = du.GetNfin();
            if (nfin <= 1) continue;
            int cnt = du.GetDeviceNum();
            if (cnt <= 0) continue;

            c_sum_weight[name] += (static_cast<double>(numerator) / static_cast<double>(nfin - 1))
                * static_cast<double>(cnt);
            c_cnt_total[name] += static_cast<long long>(cnt);
        }
    };

    for (int rr = 0; rr < Rn; ++rr) {
        if (Cn == 0) continue;
        if (Cn % 2 == 0) {
            int k = Cn / 2;
            if (k - 1 >= 0) process_side(rr, 0, k - 1, +1);
            if (Cn - 1 >= k) process_side(rr, Cn - 1, k, -1);
        }
        else {
            int m = (Cn - 1) / 2;
            process_side(rr, 0, m, +1);
            if (Cn - 1 >= m + 1) process_side(rr, Cn - 1, m + 1, -1);
        }
    }

    double c_total = 0.0;
    for (auto& kv : c_sum_weight) {
        const string& name = kv.first;
        long long tot = c_cnt_total[name];
        if (tot <= 0) continue;
        double c_avg = kv.second / static_cast<double>(tot);
        c_total += c_avg;
    }

    return r + c_total;
}

// -----------------------------------------------------------------------------
// 3. Separation Cost
// -----------------------------------------------------------------------------
double CostTableManager::CalculateSeperationCost() {
    using std::string;
    using std::vector;
    using std::unordered_map;

    struct Pt { int y, x; };
    unordered_map<string, vector<Pt>> cells;

    const int R = GetRowSize();
    const int C = GetColSize();

    const double rho_u = 0.5;
    const double l_r = 1.0;
    const double l_c = 1.0;
    if (rho_u <= 0.0 || rho_u >= 1.0) return 0.0;

    // Expand each row into unit-cells using GetDeviceOutputVector
    for (int r = 0; r < R; ++r) {
        std::vector<std::string> rowTok;
        rowTok.reserve(C * 8);
        for (int c = 0; c < C; ++c) {
            DeviceUnit& du = deviceUnitTable[r][c];
            auto v = du.GetDeviceOutputVector();
            rowTok.insert(rowTok.end(), v.begin(), v.end());
        }
        for (int i = 0; i < (int)rowTok.size(); ++i) {
            const string& name = rowTok[i];
            if (name.empty() || name == "d") continue;
            cells[name].push_back(Pt{ r, i });
        }
    }
    if (cells.size() < 2) return 0.0;

    auto rho_of = [&](const Pt& a, const Pt& b)->double {
        const double dy = (a.y - b.y) * l_r;
        const double dx = (a.x - b.x) * l_c;
        const double d = std::sqrt(dy * dy + dx * dx);
        return std::pow(rho_u, d);
    };

    // X_k: internal correlation per device
    std::unordered_map<string, double> X;
    for (auto& kv : cells) {
        const auto& pts = kv.second;
        const int n = (int)pts.size();
        double inner = 0.0;
        for (int a = 0; a < n; ++a)
            for (int b = a + 1; b < n; ++b)
                inner += rho_of(pts[a], pts[b]);
        X[kv.first] = (double)n + 2.0 * inner;
        if (X[kv.first] <= 0.0) X[kv.first] = (double)n;
    }

    std::vector<string> names; 
    names.reserve(cells.size());
    for (auto& kv : cells) names.push_back(kv.first);

    double total_sigma = 0.0;
    for (size_t i = 0; i + 1 < names.size(); ++i) {
        for (size_t j = i + 1; j < names.size(); ++j) {
            const auto& pA = cells[names[i]];
            const auto& pB = cells[names[j]];
            double numer = 0.0;
            for (const auto& a : pA)
                for (const auto& b : pB)
                    numer += rho_of(a, b);
            const double denom = std::sqrt(X[names[i]] * X[names[j]]);
            if (denom > 0.0) total_sigma += numer / denom;
        }
    }
    return total_sigma;
}
