#include <sstream>

class MakeString {
public:
    template<class T>
    MakeString& operator<< (const T& arg) {
        Stream << arg;
        return *this;
    }

    operator std::string() const {
        return Stream.str();
    }
protected:
    std::stringstream Stream;
};
