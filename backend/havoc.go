package backend

type ILoggerCore interface {
	LogInfo(fmt string, args ...any)
	LogError(fmt string, args ...any)
	LogWarn(fmt string, args ...any)
	LogDebug(fmt string, args ...any)
	LogDbgError(fmt string, args ...any)
	LogFatal(fmt string, args ...any)
	LogPanic(fmt string, args ...any)
}

type ITraceEvent interface {
	TraceEventWrite(event string, data map[string]any) error
	TraceEventRegister(callback func(event string, data map[string]any)) error
	TraceEventRemove(callback func(event string, data map[string]any)) error

	TraceAgentEventWrite(event, uuid string, data map[string]any) error
	TraceAgentEventRegister(callback func(event, uuid string, data map[string]any)) error
	TraceAgentEventRemove(callback func(event, uuid string, data map[string]any)) error
}

type IUserCore interface {
	UserList() ([]map[string]any, error)
	UserStatus(username string) int
}

type IListenerCore interface {
	ListenerNameById(id int) (string, error)
	ListenerIdByName(name string) (int, error)
	ListenerStart(name, protocol string, options map[string]any) error
	ListenerStop(name string) error
	ListenerRestart(name string) error
	ListenerRemove(name string) error
	ListenerEvent(protocol string, event map[string]any) (map[string]any, error)
	ListenerConfig(name string) (map[string]any, error)
	ListenerEdit(name string, config map[string]any) error
	ListenerList() []map[string]string
	ListenerRegisterType(name string, listener map[string]any) error
	ListenerProtocol(name string) (string, error)
	ListenerLog(name string, format string, args ...any)
	ListenerConfigPath(name string) string
	ListenerSetStatus(name string, status string)
}

type IAgentCore interface {
	AgentInitialize(uuid, plugin, target, status, note string, agent map[string]any) error
	AgentExists(uuid string) bool
	AgentType(uuid string) (string, error)
	AgentCallback(uuid string, data map[string]any)
	AgentConsole(uuid string, data map[string]any)
	AgentHeartbeat(uuid, time string)
	AgentMetadata(uuid string) (map[string]any, error)
	AgentUpdateMetadata(uuid string, data map[string]any) error
	AgentNote(uuid string) (string, error)
	AgentSetNote(uuid, note string) error
	AgentColor(uuid string) (string, error)
	AgentSetColor(uuid, color string) error
	AgentStatus(uuid string) (string, error)
	AgentSetStatus(uuid string, status string) error
	AgentSetParent(uuid string, parent string) error
	AgentProcessRequest(implant string, context map[string]any, request []byte) ([]byte, error)
	AgentRegisterCommand(implant string, register int, command func(string, any, []byte) (bool, error))
	AgentCommand(uuid string, context any, data []byte) error
	AgentPivotPoke(uuid string, context any, data []byte) error
	AgentRemove(uuid string) error
	AgentList() []string
	AgentGenerate(implant string, config map[string]any) (string, []byte, map[string]any, error)
	AgentExecute(uuid, user string, data map[string]any) (map[string]any, error)
	AgentRegisterType(name string, agent map[string]any) error
}

type ITargetCore interface {
	TargetRegister(target string, data map[string]any) error
	TargetQuery(target string) (map[string]any, error)
	TargetList() ([]string, error)
	TargetSessions(target string) ([]string, error)
}

type ITunnelCore interface {
	TunnelStart(user, host string, port int) (string, error)
	TunnelExists(socket string) bool
	TunnelChannelRead(socket string) (chan []byte, error)
	TunnelChannelWrite(socket string) (chan []byte, error)
	TunnelUser(socket string) (string, error)
	TunnelClose(socket string) error
}

type IChannelCore interface {
	ChannelRegister(name string, function func(id string)) error
	ChannelUnregister(name string) error
	ChannelNameExists(name string) bool
	ChannelNew(user, name string) (string, error)

	ChannelExists(channel string) bool
	ChannelRead(channel string) (chan []byte, error)
	ChannelWrite(channel string) (chan []byte, error)
	ChannelUser(channel string) (string, error)
	ChannelCallback(channel string) error
	ChannelDone(channel string) (<-chan struct{}, error)
	ChannelClose(channel string) error
	ChannelDelete(channel string) error
}

type IPluginCore interface {
	PluginMetaList() []map[string]any
	PluginResource(name, resource string) ([]byte, error)
	PluginCall(user, name string, arguments any) (any, error)
	PluginPath(name string) (string, error)
	PluginObject(name string) (any, error)
}

type IDatabaseCore interface {
	DatabaseAgentInsert(uuid, _type, target, parent, status, note string, metadata any) error
	DatabaseAgentUpdate(uuid, target, parent, status, note string, metadata any) error
	DatabaseAgentExists(uuid string) (bool, error)
	DatabaseAgentType(uuid string) (string, error)
	DatabaseAgentParent(uuid string) (string, error)
	DatabaseAgentStatus(uuid string) (string, error)
	DatabaseAgentNote(uuid string) (string, error)
	DatabaseAgentColor(uuid string) (string, error)
	DatabaseAgentMetadata(uuid string) ([]byte, error)
	DatabaseAgentConsole(uuid string) ([]map[string]any, error)
	DatabaseAgentDisabled(uuid string) (bool, error)
	DatabaseAgentSetDisabled(uuid string, disabled bool) error
	DatabaseAgentSetStatus(uuid string, status string) error
	DatabaseAgentSetParent(uuid string, parent string) error
	DatabaseAgentSetNote(uuid string, note string) error
	DatabaseAgentSetColor(uuid string, color string) error

	DatabaseListenerInsert(name, protocol, status string, config map[string]any) error
	DatabaseListenerSetConfig(name string, config map[string]any) error
	DatabaseListenerSetStatus(name string, status string) error
	DatabaseListenerRemove(name string) error
}

type IHavocCore interface {
	Version() map[string]string
	ConfigPath() string
	Config() map[string]any
	IsDebugMode() bool
	License() map[string]string

	ProfileLoadFile(file string) error
	ProfileLoadData(data []byte) error

	RouteHandle(pattern string, handle func(writer any, request any)) error
	RouteDelete(pattern string)

	// ILoggerCore
	// logger interface
	ILoggerCore

	// ITraceEvent
	// tracing of events for agents, listeners and other plugins
	ITraceEvent

	// IUserCore
	// user and operator session management
	IUserCore

	// IListenerCore
	// listener core apis to register, start and stop listeners
	IListenerCore

	// IAgentCore
	// agent core api which handles incoming request packages,
	// pivots, commands and processing of callbacks.
	IAgentCore

	// ITargetCore
	// target registering and association api
	ITargetCore

	// ITunnelCore
	// tunneling api to establish a connection with the client
	ITunnelCore

	// IChannelCore
	// channel api to establish an active connection
	// with the team server and its plugins
	IChannelCore

	// IPluginCore
	// interact with the plugin system
	IPluginCore

	// IDatabaseCore
	// database related functionalities, query, delete, update and
	// store data of agents, console, listeners, plugins and more.
	IDatabaseCore
}

type Plugin struct {
	// the havoc TeamServer interface, so we can interact
	// with the internal functions of the server
	IHavocCore
}

const (
	UserStatusOffline = 0
	UserStatusOnline  = 1
)

const (
	AgentCommandProcess = iota
	AgentCommandPivotPoke
)

const (
	PluginTypeListener   = "listener"
	PluginTypeAgent      = "agent"
	PluginTypeManagement = "management"
)

const (
	TraceEventUserLogin     = "user::login"
	TraceEventUserLogout    = "user::logout"
	TraceEventAgentGenerate = "agent::generate"
)

const (
	TraceAgentEventInitialize = "initialize"
	TraceAgentEventCallback   = "callback"
	TraceAgentEventTask       = "task"
	TraceAgentEventConsole    = "console"
)
