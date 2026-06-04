#pragma once

#define ARCLAB_APPLICATION_MAIN(AppType) \
    int main() \
    { \
        AppType app; \
        return app.run(); \
    }
