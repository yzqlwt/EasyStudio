
# set friendly platform define
 if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
     set(WINDOWS TRUE)
     set(SYSTEM_STRING "Windows")
 elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
     if(IOS)
         set(APPLE TRUE)
         set(SYSTEM_STRING "IOS")
     else()
         set(APPLE TRUE)
         set(MACOSX TRUE)
         set(SYSTEM_STRING "Mac OSX")
     endif()
 else()
     MESSAGE(ERROR "Unsupported platform")
 endif()

