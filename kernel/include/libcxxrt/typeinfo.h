#ifndef TYPEINFO_H
#define TYPEINFO_H

#include <stddef.h>

namespace std {
    class type_info {
    public:
        virtual ~type_info() {}
        virtual bool __do_catch(const type_info* ex_type, void** exception_object, unsigned int outer) const {
            return false;
        }
        virtual bool __do_upcast(const void* src_type, void** src_object) const {
            return false;
        }
    };
}

class __class_type_info : public std::type_info {
public:
    virtual ~__class_type_info() {}
    virtual void* cast_to(void* obj, const __class_type_info* target) const {
        return (this == target) ? obj : nullptr;
    }
    virtual bool __do_upcast(const __class_type_info* target, void** thrown_object) const {
        return false;
    }
};

class __si_class_type_info : public __class_type_info {
public:
    const __class_type_info* __base_type;

    __si_class_type_info(const __class_type_info* base) : __base_type(base) {}

    void* cast_to(void* obj, const __class_type_info* target) const override {
        if (this == target) return obj;
        return __base_type->cast_to(obj, target);
    }

    bool __do_upcast(const __class_type_info* target, void** thrown_object) const override {
        if (this == target) return true;
        return __base_type->__do_upcast(target, thrown_object);
    }
};

class __vmi_class_type_info : public __class_type_info {
public:
    struct __base_class_type_info {
        const __class_type_info* __base_type;
        ptrdiff_t __offset_flags;

        bool isVirtual() const { return __offset_flags & 0x1; }
        ptrdiff_t offset() const { return __offset_flags >> 8; }
    };

    unsigned int __base_count;
    const __base_class_type_info* __base_info;

    __vmi_class_type_info(unsigned int base_count, const __base_class_type_info* base_info)
        : __base_count(base_count), __base_info(base_info) {}

    void* cast_to(void* obj, const __class_type_info* target) const override {
        if (__do_upcast(target, &obj)) return obj;
        return nullptr;
    }

    bool __do_upcast(const __class_type_info* target, void** thrown_object) const override {
        for (unsigned int i = 0; i < __base_count; ++i) {
            const __base_class_type_info* base = &__base_info[i];
            void* adjusted_object = *thrown_object;

            if (base->isVirtual()) {
                ptrdiff_t vtable_offset = *reinterpret_cast<const ptrdiff_t*>(*static_cast<void**>(*thrown_object));
                adjusted_object = static_cast<char*>(*thrown_object) + vtable_offset;
            }

            adjusted_object = static_cast<char*>(adjusted_object) + base->offset();
            if (base->__base_type->__do_upcast(target, &adjusted_object)) {
                *thrown_object = adjusted_object;
                return true;
            }
        }
        return false;
    }
};

#endif // TYPEINFO_H
