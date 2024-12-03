#pragma once

namespace NBaseInterfaces {

class IBase {
protected:
    virtual ~IBase() = default;

public:
    struct BaseDeleter {
        void operator()(IBase* p) const { p->Delete(); }
    };

    virtual void Delete() = 0;

    IBase() = delete;
    IBase& operator=(const IBase&) = delete;
};

} // namespace NBaseInterfaces
