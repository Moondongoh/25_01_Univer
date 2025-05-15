# 컴파일러
CC := gcc

# 링크할 라이브러리
LIBS := -lpigpio -lrt

# 파일 목록 (*.c)
SRCS := $(wildcard *.c)

# 실행 파일 이름 (확장자 제거)
TARGETS := $(basename $(SRCS))

# 기본 규칙
all: $(TARGETS)

# 각 .c 파일을 개별적으로 컴파일하여 실행 파일 생성
%: %.c
	$(CC) $< -o $@ $(LIBS)

# 실행 파일 정리
clean:
	rm -f $(TARGETS)

