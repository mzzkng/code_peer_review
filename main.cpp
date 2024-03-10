#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
    // разработайте класс домена
private:
    std::string domins_;
public:
    // конструктор должен позволять конструирование из string, с сигнатурой определитесь сами
    Domain(std::string& line)
        :domins_(line){
            std::reverse(domins_.begin(),domins_.end());
            domins_.push_back('.');
    }
    // разработайте operator==
    bool operator==(const Domain& other){
        return domins_ == other.domins_;
    }
    // разработайте метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
    bool IsSubdomain(const Domain& other) const{
        if(domins_.size() < other.domins_.size()){
            return false;
        }
        auto result = std::mismatch(other.domins_.begin(), other.domins_.end(), domins_.begin());
        return result.first == other.domins_.end();
    }
    std::string getDomins() const{
        return domins_;
    }
};

class DomainChecker {
private:
    std::vector<Domain> dataBaseDomins_;
public:
    // конструктор должен принимать список запрещённых доменов через пару итераторов
    template<typename Iterator>
    DomainChecker(Iterator first, Iterator last)
        : dataBaseDomins_(first, last){
            std::sort(dataBaseDomins_.begin(), dataBaseDomins_.end(),
            [](const Domain& lhs, const Domain& rhs){
                return lhs.getDomins() < rhs.getDomins();
            });
    last = std::unique(dataBaseDomins_.begin(), dataBaseDomins_.end(),
        [](const Domain& lhs, const Domain& rhs){
            return rhs.IsSubdomain(lhs);
        });
        dataBaseDomins_.erase(last, dataBaseDomins_.end());
        }
    // разработайте метод IsForbidden, возвращающий true, если домен запрещён
    bool IsForbidden(const Domain& other) const {
        auto it = std::upper_bound(dataBaseDomins_.begin(), dataBaseDomins_.end(), other,
        [](const Domain& lhs, const Domain& rhs){
            return lhs.getDomins() < rhs.getDomins();
        });
        bool result;
        it == dataBaseDomins_.begin() ? result = false : result = other.IsSubdomain(*std::prev(it));
        return result;
    }
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
template <typename Number>
std::vector<Domain> ReadDomains(istream& in, Number num) {
    std::vector<Domain> domains;
    domains.reserve(num);
    std::string domain;
    for (size_t i = 0; i < num; ++i) {
        std::getline(in, domain);
        domains.push_back(domain);
    }
    return domains;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    std::getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}
