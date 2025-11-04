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

// Calculate all four cost terms now: CC, R, C, and Separation
std::vector<double> CostTableManager::CalculateCost()
{
    this->CCCost = this->CalculateCCCost();

    //NEW
    //why add void? because this is to avoid the compiler giving a waring of this function
    (void)this->CalculateRCCost();

    this->SeperationCost = this->CalculateSeperationCost();

    //only return R & C
    return { this->CCCost, this->RC_RCost, this->RC_CCost, this->SeperationCost };
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

    // --- R: token-level distance from row center (保持你原本加權/統計) ---
unordered_map<string, long long> r_cnt_total;
unordered_map<string, double>    r_sum_wdist;

for (int r = 0; r < Rn; ++r) {
    // 展開到 token 尺度，並保留來源 du 的 (nfin, cnt)
    std::vector<std::string> rowTokens;
    std::vector<std::pair<int,int>> token_meta; // {nfin, cnt}
    rowTokens.reserve(Cn * 4);
    token_meta.reserve(Cn * 4);

    for (int c = 0; c < Cn; ++c) {
        DeviceUnit& du = deviceUnitTable[r][c];
        auto v = du.GetDeviceOutputVector();
        for (const auto& t : v) {
            rowTokens.push_back(t);
            token_meta.emplace_back(du.GetNfin(), du.GetDeviceNum());
        }
    }

    const int W = static_cast<int>(rowTokens.size());
    if (W == 0) continue;

    const bool even = (W % 2 == 0);
    const int  k = W / 2;
    const int  m = (W - 1) / 2;

    for (int i = 0; i < W; ++i) {
        const std::string& name = rowTokens[i];
        if (name.empty() || name == "d") continue;

        double dist = even
            ? std::fabs((static_cast<double>(i) + 0.5) - static_cast<double>(k))
            : std::fabs(static_cast<double>(i - m));

        int cnt = token_meta[i].second; // 延用你原本的 deviceNum 加權
        if (cnt <= 0) continue;

        r_sum_wdist[name] += dist * static_cast<double>(cnt);
        r_cnt_total[name] += static_cast<long long>(cnt);
    }
}

std::vector<double> r_per_name_cost;
r_per_name_cost.reserve(r_sum_wdist.size());
for (auto& kv : r_sum_wdist) {
    const std::string& name = kv.first;
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


    // token-level side scan（沿用你的 numerator / seen / nfin-1 權重）
auto process_side_tokens = [&](const std::vector<std::string>& tok,
                               const std::vector<std::pair<int,int>>& meta,
                               int start, int end, int step,
                               std::unordered_map<std::string, double>&    c_sum_weight,
                               std::unordered_map<std::string, long long>& c_cnt_total) {
    std::unordered_set<std::string> seen;
    std::string prev;
    int numerator = 1;
    for (int i = start; i != end + step; i += step) {
        const std::string& name = tok[i];
        if (name.empty() || name == "d") continue;

        if (!prev.empty() && name != prev && !seen.count(name)) {
            numerator += 1;
        }
        prev = name;
        seen.insert(name);

        int nfin = meta[i].first;
        if (nfin <= 1) continue;
        int cnt = meta[i].second;
        if (cnt <= 0) continue;

        c_sum_weight[name] += (static_cast<double>(numerator) / static_cast<double>(nfin - 1))
                              * static_cast<double>(cnt);
        c_cnt_total[name]  += static_cast<long long>(cnt);
    }
};

// 逐列展開到 token 後，從左右兩側往中心掃
std::unordered_map<std::string, double>    c_sum_weight;
std::unordered_map<std::string, long long> c_cnt_total;

for (int rr = 0; rr < Rn; ++rr) {
    std::vector<std::string> rowTokens;
    std::vector<std::pair<int,int>> token_meta; // {nfin, cnt}
    rowTokens.reserve(Cn * 4);
    token_meta.reserve(Cn * 4);

    for (int c = 0; c < Cn; ++c) {
        DeviceUnit& du = deviceUnitTable[rr][c];
        auto v = du.GetDeviceOutputVector();
        for (const auto& t : v) {
            rowTokens.push_back(t);
            token_meta.emplace_back(du.GetNfin(), du.GetDeviceNum());
        }
    }

    const int W = static_cast<int>(rowTokens.size());
    if (W == 0) continue;

    if (W % 2 == 0) {
        int k = W / 2;
        if (k - 1 >= 0) process_side_tokens(rowTokens, token_meta, 0,     k - 1, +1, c_sum_weight, c_cnt_total);
        if (W - 1 >= k) process_side_tokens(rowTokens, token_meta, W - 1, k,     -1, c_sum_weight, c_cnt_total);
    } else {
        int m = (W - 1) / 2;
        process_side_tokens(rowTokens, token_meta, 0,     m,     +1, c_sum_weight, c_cnt_total);
        if (W - 1 >= m + 1)
            process_side_tokens(rowTokens, token_meta, W - 1, m + 1, -1, c_sum_weight, c_cnt_total);
    }
}

double c_total = 0.0;
for (auto& kv : c_sum_weight) {
    const std::string& name = kv.first;
    long long tot = c_cnt_total[name];
    if (tot <= 0) continue;
    double c_avg = kv.second / static_cast<double>(tot);
    c_total += c_avg;
}


    //NEW
    this->RC_RCost = r;
    this->RC_CCost = c_total;

    //Reserved
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
