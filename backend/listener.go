package backend

type IPluginListener interface {
	ListenerRegister() map[string]any
	ListenerStart(name string, options map[string]any) (map[string]string, error)
	ListenerRestore(name, status string, options map[string]any) (map[string]string, error)
	ListenerEdit(name string, config map[string]any) error
	ListenerStop(name string) (string, error)
	ListenerRestart(name string) (string, error)
	ListenerRemove(name string) error
	ListenerConfig(name string) (map[string]any, error)
}
