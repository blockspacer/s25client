
MACRO(CORRECT_LIB library name)
	IF( CMAKE_CROSSCOMPILING AND "${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin" )
		WHILE(IS_DIRECTORY ${${library}})
			STRING(REPLACE ";" " " TMP "${${library}}")
			SEPARATE_ARGUMENTS(TMP)

			SET(paths "")
			SET(new_library-NOTFOUND "not found")
			FOREACH(path ${TMP})
				IF(IS_DIRECTORY "${path}" )
					IF(EXISTS ${path}/${name})
						SET(new_library ${path}/${name})
						SET(new_library-NOTFOUND)
					ENDIF()
				ENDIF()
			ENDFOREACH()
			
			IF(NOT new_library-NOTFOUND)
				MESSAGE(STATUS "Found ${name}: ${new_library}")
				SET(${library} ${new_library})
			ENDIF()
		ENDWHILE()
	ELSE()
		MESSAGE(STATUS "Found ${name}: ${${library}}")
	ENDIF()
ENDMACRO(CORRECT_LIB)
