#ifndef HCINTERFACE_IHCAPPLICATION_H
#define HCINTERFACE_IHCAPPLICATION_H

#include <QWidget>
#include <QMainWindow>
#include <QColor>
#include <string>
#include <IHcAgent.h>

template <typename T = void>
struct HcFnCallbackHelper {
    using type = std::function<void(T)>;
};

template <>
struct HcFnCallbackHelper<void> {
    using type = std::function<void()>;
};

template <typename T = void>
using HcFnCallback = typename HcFnCallbackHelper<T>::type;

typedef struct HcCredentialsEntry {
    std::string UUID;
    std::string Username;
    std::string Password;
    std::string Domain;
    std::string Note;
    std::string Source;
    std::string Type;
    std::string TimeDate;
} HcCredentialsEntry;

class IHcLogs
{
public:
    enum level { INFO, SUCCESS, WARNING, ERROR, DEBUG };
    virtual ~IHcLogs() = default;

    virtual auto log(
        const IHcLogs::level type,
        const std::string&   msg
    ) -> void = 0;

    virtual auto log(
        const IHcLogs::level type,
        const QDateTime&     time,
        const std::string&   msg
    ) -> void = 0;

    template <typename... Args> inline
    auto info( std::format_string<Args...> fmt, Args &&...args ) -> void {
        this->log( level::INFO, std::vformat( fmt.get(), std::make_format_args( args... ) ) );
    }

    template <typename... Args> inline
    auto info( const QDateTime& time, std::format_string<Args...> fmt, Args&&... args ) -> void {
        this->log( level::INFO, time, std::vformat( fmt.get(), std::make_format_args( args... ) ) );
    };

    template <typename... Args> inline
    auto success( std::format_string<Args...> fmt, Args &&...args ) -> void {
        this->log( level::SUCCESS, std::vformat( fmt.get(), std::make_format_args( args... ) ) );
    };

    template <typename... Args> inline
    auto success( const QDateTime& time, std::format_string<Args...> fmt, Args &&...args ) -> void {
        this->log( level::SUCCESS, time, std::vformat( fmt.get(), std::make_format_args( args... ) ) );
    };

    template <typename... Args> inline
    auto warning( std::format_string<Args...> fmt, Args &&...args ) -> void {
        this->log( level::WARNING, std::vformat( fmt.get(), std::make_format_args( args... ) ) );
    };

    template <typename... Args> inline
    auto warning( const QDateTime& time, std::format_string<Args...> fmt, Args &&...args ) -> void {
        this->log( level::WARNING, time, std::vformat( fmt.get(), std::make_format_args( args... ) ) );
    };

    template <typename... Args> inline
    auto error( std::format_string<Args...> fmt, Args &&...args ) -> void {
        this->log( level::ERROR, std::vformat( fmt.get(), std::make_format_args( args... ) ) );
    };

    template <typename... Args> inline
    auto error( const QDateTime& time, std::format_string<Args...> fmt, Args &&...args ) -> void {
        this->log( level::ERROR, time, std::vformat( fmt.get(), std::make_format_args( args... ) ) );
    };

    template <typename... Args> inline
    auto debug( std::format_string<Args...> fmt, Args &&...args ) -> void {
        this->log( level::DEBUG, std::vformat( fmt.get(), std::make_format_args( args... ) ) );
    };

    template <typename... Args> inline
    auto debug( const QDateTime& time, std::format_string<Args...> fmt, Args &&...args ) -> void {
        this->log( level::DEBUG, time, std::vformat( fmt.get(), std::make_format_args( args... ) ) );
    };
};

class IHcTheme
{
public:
    virtual ~IHcTheme() = default;

    //
    // general purpose theme colors
    //

    virtual auto background() const -> QColor = 0;
    virtual auto currentline() const -> QColor = 0;
    virtual auto foreground() const -> QColor = 0;
    virtual auto comment() const -> QColor = 0;
    virtual auto cyan() const -> QColor = 0;
    virtual auto green() const -> QColor = 0;
    virtual auto orange() const -> QColor = 0;
    virtual auto pink() const -> QColor = 0;
    virtual auto purple() const -> QColor = 0;
    virtual auto red() const -> QColor = 0;
    virtual auto yellow() const -> QColor = 0;

    //
    // component specific themes (agents, listeners, etc.)
    //

    virtual auto agentGraphArrowDirect() const -> QColor = 0;
    virtual auto agentGraphArrowPivot() const -> QColor = 0;
    virtual auto agentGraphArrowUnresponsive() const -> QColor = 0;
    virtual auto agentGraphArrowDisconnected() const -> QColor = 0;
    virtual auto agentGraphItemBorder() const -> QColor = 0;

    //
    // utility functions related to theme manipulation
    //

    virtual auto getHtmlColorStart( const QColor& color ) -> QString = 0;
    virtual auto getHtmlBold() -> QString = 0;
    virtual auto getHtmlUnderline() -> QString = 0;
    virtual auto getHtmlEnd() -> QString = 0;
};

class IHcApplication {
public:
    virtual ~IHcApplication() = default;

    enum WidgetPosition {
        RightDockWidget  = 0,
        LeftDockWidget   = 1,
        BottomDockWidget = 2,
    };

    enum class TabWidgetFlags : uint32_t {
        None          = 0,
        DeleteOnClose = 1 << 0,
    };

    enum class MenuActionFlags : uint32_t {
        None           = 0,
        PositionFirst  = 1 << 0,
        PositionMiddle = 1 << 1,
        PositionLast   = 1 << 2,
        MultiSelect    = 1 << 3,

        Default = PositionMiddle | MultiSelect,
    };

    virtual auto HcVersion(
        uint32_t* major,
        uint32_t* minor,
        uint32_t* build
    ) -> void = 0;

    //
    // generic window ui apis
    //

    virtual auto HcWindowStyleSheet() -> std::string  = 0;
    virtual auto HcWindowMainWidget() -> QMainWindow* = 0;

    //
    // havoc server authentication & api methods
    //

    virtual auto HcTeamserverUser()  const -> std::string = 0;
    virtual auto HcTeamserverToken() const -> std::string = 0;
    virtual auto HcTeamserverIp()    const -> std::string = 0;
    virtual auto HcTeamserverName()  const -> std::string = 0;
    virtual auto HcTeamserverSendApi(
        const std::string& endpoint,
        const std::string& body,
        const bool         keep_alive
    ) -> std::tuple<int, std::string> = 0;

    //
    // agent api
    //

    virtual auto HcAgentObject(
        const std::string& uuid
    ) -> std::optional<IHcAgent*> = 0;

    virtual auto HcAgents(
        void
    ) -> std::vector<IHcAgent*> = 0;

    //
    // agent page apis
    //

    virtual auto HcAgentPageAddTab(
        const std::string& name,
        const QIcon&       icon,
        QWidget*           widget,
        TabWidgetFlags     flags,
    WidgetPosition         position
    ) -> void = 0;

    virtual auto HcAgentPageRegisterAction(
        const std::string& action_name,
        HcFnCallback<>     action_func,
        const QIcon&       action_icon
    ) -> void = 0;

    //
    // register agent actions
    //

    virtual auto HcAgentRegisterMenuAction(
        const std::string&       agent_type,
        const std::string&       action_name,
        HcFnCallback<IHcAgent*>  callback,
        const MenuActionFlags    flags,
        const QIcon&             icon
    ) -> void = 0;

    virtual auto HcAgentRegisterInitialize(
        const std::string&      agent_type,
        HcFnCallback<IHcAgent*> callback
    ) -> void = 0;

    virtual auto HcAgentRegisterRemove(
        const std::string&      agent_type,
        HcFnCallback<IHcAgent*> callback
    ) -> void = 0;

    virtual auto HcAgentRegisterHeartbeat(
        const std::string&      agent_type,
        HcFnCallback<IHcAgent*> callback
    ) -> void = 0;

    //
    // server page api
    //

    virtual auto HcServerPageAddWidget(
        const std::string&   name,
        const QIcon&         icon,
        const QWidget*       widget,
        const WidgetPosition position
    ) -> void = 0;

    //
    // payload/profile api
    //

    virtual auto HcPayloadGenerate(
        const std::string&   agent_type,
        const QJsonDocument& configuration
    ) -> std::vector<uint8_t> = 0;

    virtual auto HcPayloadProfiles(
        const std::optional<std::string>& agent_type
    ) -> std::vector<std::string> = 0;

    virtual auto HcPayloadGetProfile(
        const std::string& profile_name
    ) -> std::tuple<std::string, QJsonDocument> = 0;

    virtual auto HcPayloadAddProfile(
        const std::string&   agent_type,
        const std::string&   profile_name,
        const QJsonDocument& configuration
    ) -> void = 0;

    virtual auto HcPayloadDeleteProfile(
        const std::string& profile_name
    ) -> void = 0;

    //
    // credentials management
    //

    virtual auto HcCredentialsList(
        void
    ) -> std::vector<HcCredentialsEntry> = 0;

    virtual auto HcCredentialsAdd(
        const std::string& username,
        const std::string& password,
        const std::string& domain,
        const std::string& note,
        const std::string& source,
        const std::string& pass_type
    ) -> std::string = 0;

    virtual auto HcCredentialsEdit(
        const std::string& uuid,
        const std::string& username,
        const std::string& password,
        const std::string& domain,
        const std::string& note,
        const std::string& source,
        const std::string& pass_type
    ) -> void = 0;

    virtual auto HcCredentialsRemove(
        const std::string& uuid
    ) -> void = 0;

    virtual auto HcCredentialsPopup(
        void
    ) -> std::optional<HcCredentialsEntry> = 0;

    //
    // some util functions
    //

    virtual auto theme()  -> IHcTheme* = 0;
    virtual auto logger() -> IHcLogs* = 0;
};

inline auto operator | (
    IHcApplication::MenuActionFlags lhs,
    IHcApplication::MenuActionFlags rhs
) -> IHcApplication::MenuActionFlags {
    return static_cast<IHcApplication::MenuActionFlags>(
        static_cast<std::underlying_type_t<IHcApplication::MenuActionFlags>>( lhs ) |
        static_cast<std::underlying_type_t<IHcApplication::MenuActionFlags>>( rhs )
    );
}

inline auto operator & (
    IHcApplication::MenuActionFlags lhs,
    IHcApplication::MenuActionFlags rhs
) -> IHcApplication::MenuActionFlags {
    return static_cast<IHcApplication::MenuActionFlags>(
        static_cast<std::underlying_type_t<IHcApplication::MenuActionFlags>>( lhs ) &
        static_cast<std::underlying_type_t<IHcApplication::MenuActionFlags>>( rhs )
    );
}

inline auto operator ~ (
    IHcApplication::MenuActionFlags f
) -> IHcApplication::MenuActionFlags {
    return static_cast<IHcApplication::MenuActionFlags>(
        ~static_cast<std::underlying_type_t<IHcApplication::MenuActionFlags>>( f )
    );
}

inline auto operator | (
    IHcApplication::TabWidgetFlags lhs,
    IHcApplication::TabWidgetFlags rhs
) -> IHcApplication::TabWidgetFlags {
    return static_cast<IHcApplication::TabWidgetFlags>(
        static_cast<std::underlying_type_t<IHcApplication::TabWidgetFlags>>( lhs ) |
        static_cast<std::underlying_type_t<IHcApplication::TabWidgetFlags>>( rhs )
    );
}

inline auto operator & (
    IHcApplication::TabWidgetFlags lhs,
    IHcApplication::TabWidgetFlags rhs
) -> IHcApplication::TabWidgetFlags {
    return static_cast<IHcApplication::TabWidgetFlags>(
        static_cast<std::underlying_type_t<IHcApplication::TabWidgetFlags>>( lhs ) &
        static_cast<std::underlying_type_t<IHcApplication::TabWidgetFlags>>( rhs )
    );
}

inline auto operator ~ (
    IHcApplication::TabWidgetFlags f
) -> IHcApplication::TabWidgetFlags {
    return static_cast<IHcApplication::TabWidgetFlags>(
        ~static_cast<std::underlying_type_t<IHcApplication::TabWidgetFlags>>( f )
    );
}

#endif //HCINTERFACE_IHCAPPLICATION_H
