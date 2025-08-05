#ifndef HCINTERFACE_IHCAPPLICATION_H
#define HCINTERFACE_IHCAPPLICATION_H

#include <QWidget>
#include <QMainWindow>
#include <QColor>

#include <string>

#include <IHcAgent.h>

template <typename T>
using HcFnCallbackCtx = auto ( * ) ( const T& context ) -> void;
using HcFnCallback    = auto ( * ) ( void ) -> void;

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

    //
    // generic ui apis
    //

    virtual auto StyleSheet()       -> QString = 0;
    virtual auto MainWindowWidget() -> QMainWindow* = 0;

    //
    // havoc server authentication & api methods
    //

    virtual auto User()   const -> std::string = 0;
    virtual auto Token()  const -> std::string = 0;
    virtual auto Server() const -> std::string = 0;

    virtual auto ApiSend(
        const std::string& endpoint,
        const std::string& body,
        const bool         keep_alive
    ) -> std::tuple<int, std::string> = 0;


    //
    // agent page apis
    //

    virtual auto PageAgentAddTab(
        const std::string& name,
        const QIcon&       icon,
        QWidget*           widget
    ) -> void = 0;

    //
    // register actions callbacks
    //

    /*!
     * @brief
     *  register agent action
     *
     * @param action_name
     *  action name
     *
     * @param action_icon
     *  action icon
     *
     * @param action_func
     *  agent function
     *
     * @param agent_type
     *  agent type to only
     *  specify the action to
     */
    virtual auto RegisterAgentAction(
        const std::string&           action_name,
        const QIcon&                 action_icon,
        HcFnCallbackCtx<std::string> action_func,
        const std::string&           agent_type
    ) -> void = 0;

    /*!
     * @brief
     *  register agent action for
     *  all available agents
     *
     * @param action_name
     *  action name
     *
     * @param action_icon
     *  action icon
     *
     * @param action_func
     *  agent function
     */
    virtual auto RegisterAgentAction(
        const std::string&           action_name,
        const QIcon&                 action_icon,
        HcFnCallbackCtx<std::string> action_func
    ) -> void = 0;

    /*!
     * @brief
     *  register agent action for
     *  all available agents
     *
     * @param action_name
     *  action name
     *
     * @param action_func
     *  agent function
     *
     * @param multi_select
     *  add the action to
     *  the multi select
     */
    virtual auto RegisterAgentAction(
        const std::string&           action_name,
        HcFnCallbackCtx<std::string> action_func,
        bool                         multi_select
    ) -> void = 0;

    /*!
     * @brief
     *  register an action under
     *  the action menu
     *
     * @param action_name
     *  action name
     *
     * @param action_icon
     *  action icon
     *
     * @param action_func
     *  action callback
     */
    virtual auto RegisterMenuAction(
        const std::string& action_name,
        const QIcon&       action_icon,
        HcFnCallback       action_func
    ) -> void = 0;

    virtual auto RegisterServerWidget(
        const std::string&   name,
        const QIcon&         icon,
        const QWidget*       widget,
        const WidgetPosition position
    ) -> void = 0;

    virtual auto RegisterCallback(
        const std::string& callback_id,
        const std::function<void(
            const std::string agent_id,
            const std::string data,
            const std::string context
        )>& callback_fn
    ) -> void = 0;

    //
    // agent api
    //

    virtual auto Agent(
        const std::string& uuid
    ) -> std::optional<IHcAgent*> = 0;

    virtual auto Agents(
        void
    ) -> std::vector<IHcAgent*> = 0;

    //
    // python api
    //

    /*!
     * @brief
     *  run function under the context of the gil
     *
     * @param function
     *  function to run that requires the
     *  current thread context to hold the
     *  python global interpreter lock
     *
     * @param concurrent
     *  if the specified function needs to be
     *  fun under a concurrent task/thread
     *
     * @return
     *  the specified function raises any kind
     *  of python related or native exceptions
     *  it will be returned as a std::exception
     */
    virtual auto PythonContextRun(
        std::function<void()> function,
        bool                  concurrent
    ) -> void = 0;

    virtual auto Theme()  -> IHcTheme* = 0;
    virtual auto logger() -> IHcLogs* = 0;
};

#endif //HCINTERFACE_IHCAPPLICATION_H
