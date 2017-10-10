#=====================================================
# Main Nootka pro
#=====================================================

TARGET = nootka
TEMPLATE = subdirs

SUBDIRS = src/libs/core\
#           src/libs/mobile\
          src/libs/sound\
#           src/libs/misc\
#           src/libs/main\
          src\
#           src/plugins/about\
#           src/plugins/level\
#           src/plugins/settings\
#           src/plugins/wizard\
#           src/plugins/exam\
#           src/plugins/updater\


CONFIG += ordered

# src/libs/mobile.depends = src/libs/core
# src/libs/misc.depends = src/libs/widgets
# src/libs/main.depends = src/libs/widgets
src.depends = src/libs/core
sound.depends = src/libs/core
# src/plugins/about.depends = src/libs/misc
# src/plugins/level.depends = src/libs/widgets src/libs/sound
# src/plugins/settings.depends = src/libs/misc
# src/plugins/wizard.depends = src/libs/misc
# src/plugins/exam.depends = src/libs/main
# src/plugins/updater.depends = src/libs/core


 
