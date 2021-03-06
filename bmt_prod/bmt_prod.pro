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
    fx/heart.h \
    fx/metablob.h \
    fx/brain.h \
    core/fonthandler.h \
    core/pitchdetector.h \
    fx/ear.h \
    fx/credits.h \
    fx/pandazoom.h \
    util/vector2.h \
    util/matrix4.h \
    util/rect.h

SOURCES += \
    main.cpp \
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
    fx/heart.cpp \
    fx/metablob.cpp \
    fx/brain.cpp \
    core/fonthandler.cpp \
    core/pitchdetector.cpp \
    core/sound.cpp \
    fx/ear.cpp \
    fx/credits.cpp \
    fx/pandazoom.cpp \
    util/vector3.cpp

OTHER_FILES += \
    notes.txt \
    data/script.txt \
    data/shaders/lighting_directionalflat.frag \
    data/shaders/lighting_directionalflat.vert \
    data/shaders/lighting_directionaltexture.frag \
    data/shaders/lighting_directionaltexture.vert \
    data/shaders/lighting_pointflat.frag \
    data/shaders/lighting_pointflat.vert \
    data/shaders/lighting_pointtexture.frag \
    data/shaders/lighting_pointtexture.vert \
    data/shaders/lighting_singlecolor.frag \
    data/shaders/lighting_singlecolor.vert \
    data/shaders/lighting_spotlightflat.frag \
    data/shaders/lighting_spotlightflat.vert \
    data/shaders/lighting_spotlighttexture.frag \
    data/shaders/lighting_spotlighttexture.vert \
    data/shaders/postprocess_blurcombine.frag \
    data/shaders/postprocess_blurcombine.vert \
    data/shaders/postprocess_blurx.frag \
    data/shaders/postprocess_blurx.vert \
    data/shaders/postprocess_blury.frag \
    data/shaders/postprocess_blury.vert \
    data/shaders/postprocess_exp.frag \
    data/shaders/postprocess_exp.vert \
    data/shaders/postprocess_glowcombine.frag \
    data/shaders/postprocess_glowcombine.vert \
    data/shaders/heart.frag \
    data/shaders/heart.vert \
    data/shaders/metablob.frag \
    data/shaders/metablob.vert \
    data/shaders/fx_radial_blur.frag \
    data/shaders/fx_radial_blur.vert \
    data/shaders/fx_monjori.vert \
    data/shaders/fx_monjori.frag \
    data/shaders/fx_tunnel.frag \
    data/shaders/fx_tunnel.vert \
    data/shaders/postprocess_blit.frag \
    data/shaders/postprocess_blit.vert

QMAKE_CXXFLAGS += \
    -std=c++0x \
    -Wno-write-strings \ # don't warn about the deprecated conversion from string literals to char *
    -Wno-unused-parameter \
    -Wno-return-type \
    -Wno-ignored-qualifiers \
    -Wno-sign-compare \
    -Wno-unused-variable \
    -Wno-switch \
    -fno-permissive
#    -Os

LIBS += \
    -L/opt/fmodex/api/lib/ \
    -lXrandr \
    -lGLEW -lGLU -lGL \
    -lIL -lILU -lILUT \
    -lfmodex \
    -lftgl \
    -lfreetype

INCLUDEPATH += \
    /usr/include/ \
    /usr/include/c++/4.6.1/ \
    /usr/lib/gcc/x86_64-pc-linux-gnu/4.5.2/include/g++-v4/ \
    /usr/lib/gcc/x86_64-pc-linux-gnu/4.5.3/include/g++-v4/ \
    /usr/include/freetype2





























