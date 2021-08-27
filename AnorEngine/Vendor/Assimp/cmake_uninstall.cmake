IF(NOT EXISTS "H:/ProgrammingProjects/repos/GameEngineTest/Dependencies/Assimp/install_manifest.txt")
  MESSAGE(FATAL_ERROR "Cannot find install manifest: \"H:/ProgrammingProjects/repos/GameEngineTest/Dependencies/Assimp/install_manifest.txt\"")
ENDIF(NOT EXISTS "H:/ProgrammingProjects/repos/GameEngineTest/Dependencies/Assimp/install_manifest.txt")

FILE(READ "H:/ProgrammingProjects/repos/GameEngineTest/Dependencies/Assimp/install_manifest.txt" files)
STRING(REGEX REPLACE "\n" ";" files "${files}")
FOREACH(file ${files})
  MESSAGE(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  EXEC_PROGRAM(
    "C:/Users/Onuralp/Desktop/Yeni klasör (4)/cmake-3.19.2-win32-x86/bin/cmake.exe" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
    OUTPUT_VARIABLE rm_out
    RETURN_VALUE rm_retval
    )
  IF(NOT "${rm_retval}" STREQUAL 0)
    MESSAGE(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
  ENDIF(NOT "${rm_retval}" STREQUAL 0)
ENDFOREACH(file)
