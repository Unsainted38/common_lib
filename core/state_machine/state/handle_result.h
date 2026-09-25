#ifndef HANDLE_RESULT_H
#define HANDLE_RESULT_H

#include <core/state_machine/state/state_id.h>

class HandleResult {
public:
    enum class Type
    {
        Ignored,
        Handled,
        Transition
    };

    static HandleResult ignored()
    {
        return HandleResult(Type::Ignored);
    }

    static HandleResult handled()
    {
        return HandleResult(Type::Handled);
    }

    static HandleResult transitionTo(StateId target)
    {
        return HandleResult(Type::Transition, target);
    }

    Type type() const
    {
        return type_;
    }

    StateId target() const
    {
        return target_;
    }

private:
    explicit HandleResult(Type type)
        : type_(type),
        target_(StateId::Idle)
    {
    }

    HandleResult(Type type, StateId target)
        : type_(type),
        target_(target)
    {
    }

private:
    Type type_;
    StateId target_;
};

#endif // HANDLE_RESULT_H
