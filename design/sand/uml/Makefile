JAVA = java
PLANTUML_JAR = ./plantuml.jar
PLANTUML_ARGS = -o ../figs

UMLFILES = $(wildcard *.uml)

PNGFILES = $(addprefix ../figs/, $(UMLFILES:.uml=.png))

all: ${PNGFILES}

../figs/%.png : %.uml
	${JAVA} -jar ${PLANTUML_JAR} ${PLANTUML_ARGS} $<

clean:
	rm -rf ${PNGFILES}


#
