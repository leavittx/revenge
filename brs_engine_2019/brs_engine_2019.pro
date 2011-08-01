HEADERS += \
    globals.h \
    util/vector3.h \
    util/vbo.h \
    util/stringutils.h \
    util/skybox.h \
    util/range.h \
    util/primitives.h \
    util/particlesystem.h \
    util/meshfactory.h \
    util/mesh.h \
    util/matrix.h \
    util/mathematics.h \
    util/materialfactory.h \
    util/material.h \
    util/linearcurve.h \
    util/light.h \
    util/glutil.h \
    util/frustum.h \
    util/entity.h \
    util/curve.h \
    util/color4.h \
    util/color3.h \
    util/billboard.h \
    fx/tree.h \
    fx/runko.h \
    core/trigger.h \
    core/timer.h \
    core/texturehandler.h \
    core/texture.h \
    core/system.h \
    core/sound.h \
    core/shaderhandler.h \
    core/shader.h \
    core/scene.h \
    core/postprocess.h \
    core/parser.h \
    core/parameter.h \
    core/opengl.h \
    core/imagefactory.h \
    core/image.h \
    core/frametimer.h \
    core/fbo.h \
    core/event.h \
    core/demo.h \
    core/debugger.h \
    core/cubemap.h \
    core/config.h \
    core/camera.h \
    core/bpm.h \
    util/bezier.h \
    util/catmullrom.h \
    core/GLUS/GL/glus.h \
    core/keyboard.h \
    fx/heart.h

SOURCES += \
    main.cpp \
    util/vector3.cpp \
    util/vbo.cpp \
    util/stringutils.cpp \
    util/range.cpp \
    util/particlesystem.cpp \
    util/meshfactory.cpp \
    util/mesh.cpp \
    util/matrix.cpp \
    util/mathematics.cpp \
    util/materialfactory.cpp \
    util/material.cpp \
    util/linearcurve.cpp \
    util/light.cpp \
    util/glutil.cpp \
    util/frustum.cpp \
    util/entity.cpp \
    util/curve.cpp \
    util/color4.cpp \
    util/color3.cpp \
    util/billboard.cpp \
    util/bezier.cpp \
    fx/runko.cpp \
    core/trigger.cpp \
    core/timer.cpp \
    core/texturehandler.cpp \
    core/texture.cpp \
    core/system.cpp \
    core/sound.cpp \
    core/shaderhandler.cpp \
    core/scene.cpp \
    core/postprocess.cpp \
    core/parser.cpp \
    core/parameter.cpp \
    core/imagefactory.cpp \
    core/image.cpp \
    core/frametimer.cpp \
    core/fbo.cpp \
    core/event.cpp \
    core/demo.cpp \
    core/debugger.cpp \
    core/cubemap.cpp \
    core/config.cpp \
    core/camera.cpp \
    core/opengl.cpp \
    util/primitives.cpp \
    fx/tree.cpp \
    core/shader.cpp \
    core/bpm.cpp \
    util/catmullrom.cpp \
    util/skybox.cpp \
    core/opengl_linux.cpp \
    core/config_linux.cpp \
    fx/heart.cpp

OTHER_FILES += \
    notes.txt \
    data/script.txt \
    data/shaders/postprocess_glowcombine.vs \
    data/shaders/postprocess_glowcombine.fs \
    data/shaders/postprocess_exp.vs \
    data/shaders/postprocess_exp.fs \
    data/shaders/postprocess_blury.vs \
    data/shaders/postprocess_blury.fs \
    data/shaders/postprocess_blurx.vs \
    data/shaders/postprocess_blurx.fs \
    data/shaders/postprocess_blurcombine.vs \
    data/shaders/postprocess_blurcombine.fs \
    data/shaders/lighting_spotlighttexture.vs \
    data/shaders/lighting_spotlighttexture.fs \
    data/shaders/lighting_spotlightflat.vs \
    data/shaders/lighting_spotlightflat.fs \
    data/shaders/lighting_singlecolor.vs \
    data/shaders/lighting_singlecolor.fs \
    data/shaders/lighting_pointtexture.vs \
    data/shaders/lighting_pointtexture.fs \
    data/shaders/lighting_pointflat.vs \
    data/shaders/lighting_pointflat.fs \
    data/shaders/lighting_directionaltexture.vs \
    data/shaders/lighting_directionaltexture.fs \
    data/shaders/lighting_directionalflat.vs \
    data/shaders/lighting_directionalflat.fs \
    data/shaders/heart.fs \
    data/shaders/heart.vs

QMAKE_CXXFLAGS += \
    -std=c++0x \
    -Wno-write-strings \ # don't warn about the deprecated conversion from string literals to char *
    -Wno-unused-parameter \
    -Wno-return-type \
    -Wno-ignored-qualifiers \
    -Wno-sign-compare \
    -Wno-unused-variable \
    -fno-permissive
#    -Os

LIBS += \
    -L/opt/fmodex/api/lib/ \
    -lXrandr \
    -lGLEW -lGLU -lGL \
    -lIL -lILU -lILUT \
    -lfmodex

INCLUDEPATH += \
    /usr/include/ \
    /usr/include/c++/4.6.1/ \
    /usr/lib/gcc/x86_64-pc-linux-gnu/4.5.2/include/g++-v4/
#    /usr/lib/gcc/x86_64-pc-linux-gnu/4.6.1/include/g++-v4/
