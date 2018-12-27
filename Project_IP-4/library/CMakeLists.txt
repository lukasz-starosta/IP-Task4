cmake_minimum_required(VERSION 3.4)
project(Library)

set(SOURCE_FILES
    include/Core/CImg.h
    include/Core/ArgumentsHandler.h
    include/Core/Error.h
    include/ImageProcesser.h
    include/HistogramProcesser.h
	include/MorphologicalProcesser.h
    src/Core/ArgumentsHandler.cpp
    src/Core/Error.cpp
    src/Core/Processer.cpp
	src/HistogramProcesser/HistogramProcesser.cpp
	src/HistogramProcesser/HistogramCreator.cpp
	src/HistogramProcesser/ImageCharacteristicsCalculator.cpp
	src/HistogramProcesser/EdgeSharpening.cpp
	src/HistogramProcesser/Rosenfeld.cpp
	src/HistogramProcesser/HPower.cpp
    src/ImageProcesser/ImageProcesser.cpp
    src/ImageProcesser/BasicImageTransformations.cpp
    src/ImageProcesser/HelperMethods.cpp
    src/ImageProcesser/ImageAnalyzers.cpp
    src/ImageProcesser/ImageFilters.cpp
	src/MorphologicalOperations/MorphologicalProcesser.cpp
	src/MorphologicalOperations/MorphologicalOperations.cpp
	src/MorphologicalOperations/RegionMerging.cpp
)

add_library(Library ${SOURCE_FILES})
target_include_directories(Library PUBLIC include 
	PUBLIC include/Core)