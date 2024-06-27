# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/AJChen2005/esp/v5.2.1/esp-idf/components/bootloader/subproject"
  "C:/Users/AJChen2005/Documents/GitHub/NTUEECamp_2024_Gameconsole/Speaker/pipeline_spiffs_mp3/build/bootloader"
  "C:/Users/AJChen2005/Documents/GitHub/NTUEECamp_2024_Gameconsole/Speaker/pipeline_spiffs_mp3/build/bootloader-prefix"
  "C:/Users/AJChen2005/Documents/GitHub/NTUEECamp_2024_Gameconsole/Speaker/pipeline_spiffs_mp3/build/bootloader-prefix/tmp"
  "C:/Users/AJChen2005/Documents/GitHub/NTUEECamp_2024_Gameconsole/Speaker/pipeline_spiffs_mp3/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/AJChen2005/Documents/GitHub/NTUEECamp_2024_Gameconsole/Speaker/pipeline_spiffs_mp3/build/bootloader-prefix/src"
  "C:/Users/AJChen2005/Documents/GitHub/NTUEECamp_2024_Gameconsole/Speaker/pipeline_spiffs_mp3/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/AJChen2005/Documents/GitHub/NTUEECamp_2024_Gameconsole/Speaker/pipeline_spiffs_mp3/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/AJChen2005/Documents/GitHub/NTUEECamp_2024_Gameconsole/Speaker/pipeline_spiffs_mp3/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
