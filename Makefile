.PHONY: all clean run runt

all: exe/main.exe

docs:
	@doxygen Doxyfile

run: all
	@exe/main.exe

clean:
	@rmdir d /s /q
	@rmdir o /s /q
	@rmdir exe /s /q
	@mkdir d
	@mkdir o
	@mkdir exe

CC:= gcc

O_FILES:= $(patsubst %.cpp,o/%.o,$(notdir $(wildcard src/*.cpp)))

DED_FLAGS := -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wpointer-arith -Wstack-usage=8192 -Wstrict-aliasing -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

CFLAGS:= -I ./h $(DED_FLAGS) -Wno-unused-parameter

exe/main.exe: $(O_FILES)
	@$(CC) $(O_FILES) -o exe/main.exe

include $(wildcard d/*.d)

o/%.o: src/%.cpp
	@$(CC) $< $(CFLAGS) -c -o $@
	@$(CC) -MM -MT $@ -I ./h $< -o d/$(patsubst %.o,%.d,$(notdir $@))



# .PHONY: all clean run runt

# all: exe/main.exe

# docs:
# 	@doxygen Doxyfile

# run: 
# 	@exe/main.exe

# clean:
# 	@rmdir d /s /q
# 	@rmdir o /s /q
# 	@rmdir exe /s /q
# 	@mkdir d
# 	@mkdir o
# 	@mkdir exe

# O_FILES:= $(patsubst %.cpp,o/%.o,$(notdir $(wildcard src/*.cpp)))

# DED_FLAGS := -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wpointer-arith -Wstack-usage=8192 -Wstrict-aliasing -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

# CFLAGS:= -I ./h $(DED_FLAGS)

# CC:=gcc

# exe/main.exe: $(O_FILES)
# 	@$(CC) $(O_FILES) -o exe/main.exe

# include $(wildcard d/*.d)

# o/%.o: src/%.cpp
# 	@$(CC) $< $(CFLAGS) -c -o $@
# 	@$(CC) -MM -MT $@ -I ./h $< -o d/$(patsubst %.o,%.d,$(notdir $@))