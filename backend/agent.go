package backend

var (
	AgentStatusHealthy      = "healthy"
	AgentStatusDisconnected = "disconnected"
	AgentStatusUnresponsive = "unresponsive"
	AgentStatusRemoved      = "removed"
)

type IPluginAgent interface {
	AgentRegister() map[string]any
	AgentRestore(uuid, parent, status, note string, serialized []byte) error
	AgentRemove(uuid string) error
	AgentUpdate(uuid string) error
	AgentGenerate(config map[string]any) (string, []byte, map[string]any, error)
	AgentExecute(uuid, user string, data map[string]any) (map[string]any, error)
	AgentProcess(context map[string]any, request []byte) ([]byte, error)
	AgentMetadata(uuid string) (map[string]any, error)
	AgentObject(uuid string) (any, error)
}
