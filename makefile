#C++向けで、以下の前提で使用可能なmakefile
#
#・同一フォルダにヘッダ・ソースと実行ファイルが格納されている
#・実行ファイルはフォルダ名と同一
#・main関数を持つソースはフォルダ名.cpp
#・ソースの拡張子はcpp

#変数定義
###################################################

#ターゲット
#作成する実行ファイル
#2個上のフォルダ名と同一
TARGET := $(shell basename `pwd`)

#テスト用のターゲット
#テストを実行する
TESTTARGET := unittest

#NVCC、g++共通のフラグ（デバッグ出力、最適化レベル）:デバッグ用
COMMON_FLAGS := -pg -g -O0
#COMMON_FLAGS := 

#g++のコンパイルのフラグ
#CXXFLAGS := -Wall -lsqlite3 -L/usr/lib/x86_64-linux-gnu -lglut -lGLEW -lGLU -lGL -I/usr/include
CXXFLAGS := -Wall -lsqlite3 -L/usr/lib/nvidia-367 -lglut -lGLEW -lGLU -lGL -I/usr/include
CXXONLYFLAGS :=  -fopenmp -std=c++11

#CUDA関連の設定

#CUDAコンパイラ
NVCC := nvcc

#CUDAコンパイルオプション
NVCC_FLAGS :=  -Xcompiler -Wall -Xcompiler -fopenmp

#CUDAリンクオプション
NVCC_LINKS := -lcuda -lcudart


#ソースファイル
#フォルダ直下の.cppファイル
SRCS := $(shell ls *.cpp)

#CUDAソース
#フォルダ直下の.cuファイル
CUDA_SRCS := $(shell ls *.cu)

#オブジェクトファイル
#.cpp or .cuを.oに置換している
OBJS := $(SRCS:.cpp=.o) $(CUDA_SRCS:.cu=.o)

#ターゲット用のオブジェクトファイル
OBJSTARGET := $(filter-out $(TESTTARGET).o, $(OBJS))

#テスト用のオブジェクトファイル
OBJSTEST := $(filter-out $(TARGET).o, $(OBJS))

#依存関係ファイル
DEPS := $(SRCS:.cpp=.d) $(CUDA_SRCS:.cu=.d)

#コマンド
###################################################

#全てコンパイルする
#先頭に書いたコマンドはmakeコマンドでオプション無しで実行される
.PHONY: all
all: $(TARGET)
#all: $(TARGET) $(TESTTARGET)

.PHONY: run
run: $(TARGET)
	./$(TARGET)
	gprof $(TARGET) | ./gprof2dot.py | dot -Tpng -o gprof_call_graph_$(TARGET).png

#テストの実行ファイルを実行する
#ログと標準出力両方に出力する
#.PHONY: test
#test: $(TESTTARGET)
#	./$(TESTTARGET) 2>&1 | tee -a $(TESTTARGET).log
#	gprof $(TESTTARGET) | ./gprof2dot.py | dot -Tpng -o gprof_call_graph_$(TESTTARGET).png

#実行ファイルの作成
#オプションは最後に指定する
#先頭に書くとコンパイルが通らない
#<ターゲット>: ターゲットに依存するファイル
#$@ : ターゲット名
#$< : 最初の依存ファイル名(":"の右側の最初(一番左)の項目)
#$^ : 依存ファイル全体
$(TARGET): $(OBJSTARGET)
	$(NVCC) $(NVCC_LINKS) $(COMMON_FLAGS) $(NVCC_FLAGS) -o $@ $^ -Xcompiler $(CXXFLAGS)

#テスト用の実行ファイルを作成
#$(TESTTARGET): $(OBJSTEST)
#	$(NVCC) $(NVCC_LINKS) $(COMMON_FLAGS) $(NVCC_FLAGS) -o $@ $^ -Xcompiler $(CXXFLAGS)

#オブジェクトファイルの作成
#"-MMD"オプションによって依存関係ファイル(*.d)も同時に作成する
%.o: %.cpp
	$(CXX) -c -MMD $< $(COMMON_FLAGS) $(CXXONLYFLAGS) $(CXXFLAGS)

%.o: %.cu
	$(NVCC) $(COMMON_FLAGS) $(NVCC_LINKS) $(NVCC_FLAGS) -c $<
	$(NVCC) $(COMMON_FLAGS) $(NVCC_LINKS) $(NVCC_FLAGS) -M $< > $*.d

#実行ファイル・中間ファイルの削除
#".PHONY"はターゲットではなくコマンドであることを明示するためのコマンド
.PHONY: clean
clean:
	$(RM) $(TARGET) $(TESTTARGET) $(OBJS) $(DEPS)

#依存関係ファイルをインクルードする
#先頭に”-”を書いているので依存関係ファイルがなくてもエラーとならない
-include $(DEPS)
