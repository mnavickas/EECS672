CPP = g++
# Needs full cryph because it uses ProjPoint:
INC = -I../cryphutil -I../glslutil -I../imageutil -I../mvcutil
C_FLAGS = -fPIC -g -c -DGL_GLEXT_PROTOTYPES $(INC)

LINK = $(CPP) -fPIC -g
# Needs full cryph because it uses ProjPoint:
LOCAL_UTIL_LIBRARIES = ../lib/libcryph.so ../lib/libglsl.so ../lib/libimage.so ../lib/libmvc.so
OGL_LIBRARIES = $(GL_LIB_LOC) -lglfw -lGLU -lGL
LIBS = $(LOCAL_UTIL_LIBRARIES) $(OGL_LIBRARIES)

OBJS = tribezier.o TriBezierModel.o ModelView_Additions.o

tribezier: $(OBJS) $(LOCAL_UTIL_LIBRARIES)
	$(LINK) -g -o tribezier $(OBJS) $(LIBS)

../lib/libcryph.so: ../cryphutil/AffPoint.h ../cryphutil/AffPoint.c++
	(cd ../glslutil; make)

../lib/libglsl.so: ../glslutil/ShaderIF.h ../glslutil/ShaderIF.c++
	(cd ../glslutil; make)

../lib/libimage.so: ../imageutil/ImageReader.h ../imageutil/ImageReader.c++
	(cd ../imageutil; make)

../lib/libmvc.so: ../mvcutil/Controller.c++ ../mvcutil/ModelView.c++
	(cd ../mvcutil; make)

tribezier.o: tribezier.c++
	$(CPP) $(C_FLAGS) tribezier.c++
TriBezierModel.o: TriBezierModel.h TriBezierModel.c++
	$(CPP) $(C_FLAGS) TriBezierModel.c++
ModelView_Additions.o: ModelView_Additions.c++
	$(CPP) $(C_FLAGS) ModelView_Additions.c++
