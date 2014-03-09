#ifdef __cplusplus
extern "C" {
#endif

bool _getDefaultDevice(char **id, char **name);

int _getNumSystemDevices();
void _getSystemDevices(char **IDs, char **names);

void *_getDeviceHandle(const char *id);
void _releaseDeviceHandle(void *handle);

void *_getSessionHandle();
void _releaseSessionHandle(void *handle);
void _startSession(void *handle);
void _stopSession(void *handle);
void *_setupSessionInput(void *device, void *session);
void _releaseSessionInput(void *input, void *session);
void *_setupSessionOutput(void *session);
void _releaseSessionOutput(void *output, void *session);
const unsigned char *_getSnapshot(void *output, int &len);

#ifdef __cplusplus
}
#endif
