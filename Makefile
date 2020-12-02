# 各種ファイルの保存ディレクトリ
DIR_SRC  := ./src
DIR_HEAD := ./include
DIR_OBJ  := ./build
DIR_IMG  := ./img

# 生成するプログラム
PRGS := 
PRGS += test_io test_uss test_request test_direction
PRGS += test_capture test_number test_dir_num
PRGS += test_server test_position
PRGS += pilot
#新規追加
PRGS += test_line

# オブジェクト
OBJS_RUN := $(addprefix $(DIR_OBJ)/, io.o uss.o arduino.o request.o direction.o)
OBJS_CAP := $(addprefix $(DIR_OBJ)/, get_img.o)
OBJS_NUM := $(addprefix $(DIR_OBJ)/, number_search.o number_detect.o)
OBJS_SVR := $(addprefix $(DIR_OBJ)/, server.o)
OBJS_POS := $(addprefix $(DIR_OBJ)/, position.o)
# 新規追加
OBJS_LINE := $(addprefix $(DIR_OBJ)/, test_line.o request.o arduino.o)

# コンパイルオプション
CC      := g++
G++      := g++
CFLAGS  := -O2 -Wall -MMD -MP `pkg-config --cflags opencv`
LDFLAGS := -lm -lpthread -lwiringPi `pkg-config --libs opencv`
INCLUDE := -I $(DIR_HEAD)

# 全コンパイル・ファイル削除用ターゲット
.PHONY: all clean

all: $(PRGS)

clean:
	$(RM) $(PRGS)
	$(RM) $(DIR_OBJ)/*.o $(DIR_OBJ)/*.d
	$(RM) $(DIR_IMG)/*.jpg

# パターンルール
$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@if [ ! -d $(DIR_OBJ) ]; then echo "mkdir $(DIR_OBJ)"; mkdir $(DIR_OBJ); fi
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.cpp
	@if [ ! -d $(DIR_OBJ) ]; then echo "mkdir $(DIR_OBJ)"; mkdir $(DIR_OBJ); fi
	$(G++) -c $(CFLAGS) $(INCLUDE) $< -o $@

# 依存関係ファイルの読み込み
-include $(DIR_OBJ)/*.d

# プログラム毎に生成ルールを記述
pilot: $(OBJS_RUN) $(OBJS_CAP) $(OBJS_NUM) $(OBJS_CIR) $(OBJS_SVR) $(OBJS_POS) $(DIR_OBJ)/pilot.o
	$(CC) $(LDFLAGS) $^ -o $@

test_io: $(OBJS_RUN) $(DIR_OBJ)/test_io.o
	$(CC) $(LDFLAGS) $^ -o $@

test_uss: $(OBJS_RUN) $(DIR_OBJ)/test_uss.o
	$(CC) $(LDFLAGS) $^ -o $@

test_request: $(OBJS_RUN) $(DIR_OBJ)/test_request.o
	$(CC) $(LDFLAGS) $^ -o $@

test_direction: $(OBJS_RUN) $(DIR_OBJ)/test_direction.o
	$(CC) $(LDFLAGS) $^ -o $@

test_capture: $(OBJS_CAP) $(DIR_OBJ)/test_capture.o
	$(CC) $(LDFLAGS) $^ -o $@

test_number: $(OBJS_CAP) $(OBJS_NUM) $(DIR_OBJ)/test_number.o
	$(CC) $(LDFLAGS) $^ -o $@

test_dir_num: $(OBJS_RUN) $(OBJS_CAP) $(OBJS_NUM) $(DIR_OBJ)/test_dir_num.o
	$(CC) $(LDFLAGS) $^ -o $@

test_server: $(OBJS_SVR) $(DIR_OBJ)/test_server.o
	$(CC) $(LDFLAGS) $^ -o $@

test_position: $(OBJS_POS) $(DIR_OBJ)/test_position.o
	$(CC) $(LDFLAGS) $^ -o $@

test_line : $(OBJS_LINE) $(DIR_OBJ)/test_line.o
	$(CC) $(LDFLAGS) $^ -o $@

