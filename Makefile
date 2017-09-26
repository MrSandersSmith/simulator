CXX=g++

VPATH += :./Element
VPATH += :./Scene
VPATH += :./Utils
VPATH += :./

#files
OFILES += Element.o
OFILES += Scene.o
OFILES += config.o
OFILES += main.o

CXXFLAGS =

LINKFLAGS = -L/usr/lib/x86_64-linux-gnu -lGL -lglut -lGLU

MAIN=apps/simulator
OFILES_PATH=$(addprefix obj/,$(OFILES))
INCLUDE=$(subst :, -I,$(VPATH))

all: $(MAIN) end

$(MAIN): start mk_objdir mk_trgtdir $(OFILES_PATH)
	$(CXX) $(INCLUDE) -o $(MAIN) $(OFILES_PATH) $(LINKFLAGS)

start:
	@echo ================ Begin ================

end:
	@echo ================ End ==================

mk_objdir:
	@mkdir -p obj
	@cd obj

mk_trgtdir:
	@mkdir -p apps

obj/%.o : %.cpp
	$(CXX) $(INCLUDE) -c $(CXXFLAGS) $< -o $@

clean:
	rm -rf obj

cleanall:
	rm -rf obj apps