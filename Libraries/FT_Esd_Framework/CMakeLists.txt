
FILE(GLOB SRCS *.c)
FILE(GLOB HDRS *.h)
SOURCE_GROUP("" FILES ${SRCS} ${HDRS})

ADD_LIBRARY(FT_Esd_Framework
	${SRCS}
	${HDRS}
)

TARGET_LINK_LIBRARIES(FT_Esd_Framework
	FT_Eve_Hal
)

SET_TARGET_PROPERTIES(FT_Esd_Framework PROPERTIES FOLDER "[X] Distrib Libraries")
