#ifndef IHCAGENT_H
#define IHCAGENT_H

#include <QJsonDocument>
#include <string>

enum HcAgentStatus {
    AgentStatusHealthy,
    AgentStatusUnresponsive,
    AgentStatusDisconnected,
    AgentStatusRemoved,
};

class IHcAgent {
public:
    virtual ~IHcAgent() = default;

    virtual auto uuid() -> std::string = 0;
    virtual auto type() -> std::string = 0;

    virtual auto data() -> QJsonDocument = 0;
    virtual auto python_object() -> PyObject* = 0;

    virtual auto parent() -> std::string = 0;
    virtual auto set_parent(
        const std::string& parent
    ) -> void = 0;

    virtual auto status() -> std::string = 0;
    virtual auto set_status(
        HcAgentStatus status
    ) -> void = 0;

    virtual auto set_last(
        const std::string& last
    ) -> QDateTime = 0;
    virtual auto last() -> QDateTime = 0;

    virtual auto hidden() -> bool = 0;
    virtual auto set_hidden(
        bool hidden
    ) -> void = 0;

    virtual auto image() -> QImage = 0;
    virtual auto set_image(
        const QImage& image
    ) -> void = 0;

    virtual auto write_console(
        const std::string& text
    ) -> void = 0;

    virtual auto remove() -> void = 0;
};

#endif //IHCAGENT_H
