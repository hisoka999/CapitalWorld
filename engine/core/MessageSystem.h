/*
 * MessageSystem.h
 *
 *  Created on: 17.01.2016
 *      Author: stefan
 */

#ifndef CORE_MESSAGESYSTEM_H_
#define CORE_MESSAGESYSTEM_H_

#include <any>
#include <memory>
#include <engine/core/Consumer.h>
#include <engine/core/Message.h>
#include <engine/core/dispatcher.h>
#include <iostream>
#include <unordered_map>
#include <vector>

struct MessageDispatcher {
    core::dispatcher_type dispatcher;
    int reference;
};

namespace core {
template <typename Type>
class MessageSystem {
public:
    MessageSystem()
        : reference_count(0)
    {
    }
    virtual ~MessageSystem()
    {
        _consumer.clear();
    }

    static MessageSystem& get()
    {
        static MessageSystem instance;
        return instance;
    }
    void deregister(int reference)
    {
        for (auto it = _consumer.begin(); it != _consumer.end(); ++it) {
            if (it->second.reference == reference) {
                _consumer.erase(it);
                return;
            }
        }
    }

    template <typename F>
    int registerForType(Type pType, F&& f)

    {
        reference_count++;
        MessageDispatcher disp;
        disp.dispatcher = core::make_dispatcher(std::forward<F>(f));
        disp.reference = reference_count;
        _consumer.emplace(pType, disp);
        return disp.reference;
    }
    template <typename Data>
    void sendMessage(const std::shared_ptr<Message<Type, Data>> pMessage)
    {
        //std::cout << "send message" << std::endl;

        auto rng = _consumer.equal_range(pMessage->getType());
        for (auto it = rng.first; it != rng.second; ++it) {
            std::cout << "send message to ref: " << it->second.reference
                      << std::endl;
            call(it->second.dispatcher, pMessage->getData());
        }
    }

private:
    template <typename F, typename... Args>
    void call(F const& f, Args const&... args)
    {
        std::vector<std::any> v { args... };
        f(v);
    }
    std::unordered_multimap<Type, MessageDispatcher> _consumer;
    int reference_count;
};

} /* namespace core */

#endif /* CORE_MESSAGESYSTEM_H_ */
