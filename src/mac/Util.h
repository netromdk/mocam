#ifdef __cplusplus
extern "C" {
#endif

bool _getDefaultDevice(char **id, char **name);

int _getNumSystemDevices();
void _getSystemDevices(char **IDs, char **names);

void *_getDeviceHandle(const char *id);
void _releaseDeviceHandle(void *handle);

#ifdef __cplusplus
}
#endif
