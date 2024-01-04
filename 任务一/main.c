#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG 0

int poolsize = 256 * 1024;
char keywords[] = "PROGRAM BEGIN END CONST VAR WHILE DO IF THEN";
char* src; // 指向源代码

char text[9096] = "";
int text_p = 0;

// 表明这个符号的类型
enum {
	Num = 128, Main, Fun, Glo, Loc, Id, Defined,
	Begin, End, Const, Var, While, Do, If, Then,
	Assign,
	Eq, Ne, Lt, Gt, Le, Ge, Right, Add, Sub, Mul, Div, Left,
};

enum { Token, Hash, Name, Type, Class, Value, IdSize };
long long* current_id, // current parsed ID
* symbols; // symbol table
char* data,
* current_data;
long long token, token_val;

char* code;
int NameCounter = 0;

// 符号栈
long long* symbol_stack, * ssp;
// 算符栈
int operator_stack[100], osp=0;

// 行数统计
int line = 0;

void PrintSymbolTable();
void CompoundStatement();

char* NameGenerator() {
	char* p;
	p = current_data;
	// 生成一个像 t_1 t_2 t_3 这样的名字
	NameCounter++;
	*current_data = 't';
	*++current_data = '_';
	// 将 NameCounter 转换成字符串 向后填入 current_data
	// 获取 NameCounter 的位数
	int temp = NameCounter;
	int count = 1;
	while (temp != 0) {
		temp /= 10;
		count *= 10;
	}
	count /= 10;
	temp = NameCounter;
	do {

		*++current_data = temp / count + '0';
		temp %= count;
		count /= 10;
	} while (count != 0);
	*++current_data = ' ';
	++current_data;
	return p;
}

char* TramformNumToName(int num) {
	if (num == 0) {
		*current_data++ = '0';
		*current_data++ = ' ';
		return current_data - 2;
	}
	char* p;
	p = current_data;
	// 将 NameCounter 转换成字符串 向后填入 current_data
	// 获取 NameCounter 的位数
	int temp = num;
	int count = 1;
	while (temp != 0) {
		temp /= 10;
		count *= 10;
	}
	count /= 10;
	temp = num;
	do {

		*current_data++ = temp / count + '0';
		temp %= count;
		count /= 10;
	} while (count != 0);
	*current_data++ = ' ';
	return p;
}

void PrintOutRealName(char* p) {
	char* temp = p;
	while ((*temp >= 'a' && *temp <= 'z') || (*temp >= 'A' && *temp <= 'Z') || (*temp >= '0' && *temp <= '9') || (*temp == '_')) {
		text[text_p++] = *temp++;
		text[text_p] = '\0';
	}
}

void StoreText(char* str) {
	int i = 0;
	while (str[i] != '\0') {
		text[text_p++] = str[i++];
	}
	text[text_p] = '\0';
}

void PrintOutRealOperator(int op) {
	switch (op) {
	case Add:
		StoreText("+");
		break;
	case Sub:
		StoreText("-");
		break;
	case Mul:
		StoreText("*");
		break;
	case Div:
		StoreText("/");
		break;
	case Eq:
		StoreText("==");
		break;
	case Ne:
		StoreText("!=");
		break;
	case Lt:
		StoreText("<");
		break;
	case Gt:
		StoreText(">");
		break;
	case Le:
		StoreText("<=");
		break;
	case Ge:
		StoreText(">=");
		break;
	case Assign:
		StoreText(":=");
		break;
	case Left:
		StoreText("(");
		break;
	case Right:
		StoreText(")");
		break;
	}
}

void InsertNumAt(int num, int pos) {
	// 获取num的位数
	int temp = num;
	int weight = 1;
	int count = 1;
	while (temp != 0) {
		temp /= 10;
		weight++;
		count *= 10;
	}
	// 将text从pos开始往后移动count位
	int i = text_p;
	while (i >= pos) {
		text[i + weight - 1] = text[i];
		i--;
	}
	text_p += weight - 1;
	count /= 10;
	temp = num;
	do {
		text[pos-1] = temp / count + '0';
		pos++;
		temp %= count;
		count /= 10;
	} while (count != 0);
	text[pos-1] = '\n';
}

void next() {
	char* begin_pos;
	int hash;

	// 读完了
	if (src == '\0' || *src == EOF) {
		token = 0;
		return;
	}
	while (token = *src) {
		++src;
		if (token == '\n') {

		}
		else if (token >= 'a' && token <= 'z' || token >= 'A' && token <= 'Z' || token == '_') {
			begin_pos = src - 1; // 这个东西的名称的开始的地方
			hash = token;

			// 读完整个名称
			while ((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_')) {
				hash = hash * 147 + *src;
				src++;
			}


			current_id = symbols; // 指向符号表的开始
			while (current_id[Token]) {
				// 看看是不是已经存在了
				if (current_id[Hash] == hash) {
					token = current_id[Token];
					return;
				}
				current_id = current_id + IdSize; // 指向下一个符号
			}

			// 到这里说明没有找到，那么就是新的符号 
			// char * 是8字节，转 long long
			current_id[Hash] = hash;
			current_id[Name] = (long long)begin_pos; // 名称的开始的地方 直接就是那个常量的地址
			token = current_id[Token] = Id; // token的值
			return;
		}
		else if (token >= '0' && token <= '9') {
			// 只支持正十进制
			token_val = token - '0';
			if (token_val > 0) {
				// dec, starts with [1-9]
				while (*src >= '0' && *src <= '9') {
					token_val = token_val * 10 + *src++ - '0';
				}
			}

			token = Num;
			return;
		}

		else if (token == '+') { 
			token = Add;
			return;
		}
		else if (token == '-') {
			token = Sub;
			return;
		}
		else if (token == '*') {
			token = Mul;
			return;
		}
		else if (token == '/') {
			if (*src == '/') {
				src++;
				// skip comments
				while (*src != 0 && *src != '\n') {
					++src;
				}
			}
			else {
				token = Div;
				return;
			}
		}
		else if (token == '=') {
				token = Eq;
				return;
		}
		else if (token == '<' && *src == '>') {
			src++;
			token = Ne;
			return;
		}
		else if (token == '<') {
			if (*src == '=') {
				src++;
				token = Le;
				return;
			}
			else {
				token = Lt;
				return;
			}
		}
		else if (token == '>') {
			if (*src == '=') {
				src++;
				token = Ge;
				return;
			}
			else {
				token = Gt;
				return;
			}
		}
		else if (token == ':' && *src == '=') {
			src++;
			token = Assign;
			return;
		}
		else if (token == '(') {
			token = Left;
			return;
		}
		else if (token == ')') {
			token = Right;
			return;
		}
		else if (token == ';' || token == ','
			|| token == '{' || token == '}') {
			return;
		}
	}
}

void match(int tk) {
	if (token == tk) {
		next();
	}
	else {
		printf("expected token: %d\n", tk);
		PrintSymbolTable();
		exit(-1);
	}
}

/*
	递归下降分析开始
*/

char* expression() {
	if (token == Sub) {
		next();

		char* t = TramformNumToName(++line);
		PrintOutRealName(t);
		StoreText(" ");

		t = NameGenerator();
		*ssp = t;
		ssp++;
		PrintOutRealName(t);
		StoreText(" := -");
		PrintOutRealName(current_id[Name]);
		StoreText("\n");
		next();
	}
	while (1) {

		if (token == Id) {
			if (current_id[Type] != Defined) {
				printf("undefined symbol!");
				exit(-1);
			}
			*ssp = current_id[Name];
			ssp++;
		}
		else if (token == Num) {
			*ssp = TramformNumToName(token_val);
			ssp++;
		}
		else if (token == Left) {
			operator_stack[osp] = Left;
			osp++;
		}
		// 算符
		else if (token == Add || token == Sub || token == Mul || token == Div || token == Right) {
			if (osp == 0) {
				operator_stack[osp] = token;
				osp++;
			}
			else if (token <= operator_stack[osp - 1]) {
				// 说明栈顶的优先级比较高
				// 从栈顶开始计算
				while (osp != 0 && token <= operator_stack[osp - 1]) {
					if (operator_stack[osp - 1] == Left) {
						if (token == Right) {
							// 说明是括号
							osp -= 1;
						}
						break;
					}
					char* t = TramformNumToName(++line);
					PrintOutRealName(t);
					StoreText(" ");

					t = NameGenerator();
					PrintOutRealName(t);
					StoreText(" := ");
					PrintOutRealName(*(ssp - 2));
					StoreText(" ");
					PrintOutRealOperator(operator_stack[osp - 1]);
					StoreText(" ");
					PrintOutRealName(*(ssp - 1));
					StoreText("\n");
					// 计算完了之后，将结果放到栈顶
					*(ssp - 2) = (long long)t;
					ssp -= 1;
					osp -= 1;
				}
				if (token != Right) {
					operator_stack[osp] = token;
					osp++;
				}
			}
			else {
				operator_stack[osp] = token;
				osp++;
			}
		}
		else {
			break;
		}
		next();
	}

	// 最后计算栈里面的东西
	while (osp != 0) {
		char* t = NameGenerator();

		char* t1 = TramformNumToName(++line);
		PrintOutRealName(t1);
		StoreText(" ");

		PrintOutRealName(t);
		StoreText(" := ");
		PrintOutRealName(*(ssp - 2));
		StoreText(" ");
		PrintOutRealOperator(operator_stack[osp - 1]);
		StoreText(" ");
		PrintOutRealName(*(ssp - 1));
		StoreText("\n");
		// 计算完了之后，将结果放到栈顶
		*(ssp - 2) = (long long)t;
		ssp -= 1;
		osp -= 1;
	}
	// 清空栈
	ssp = symbol_stack;
	osp = 0;
	// printf("%c", *(char*)*symbol_stack);
	return *symbol_stack; // 因为最后一次算得的符号肯定会放在栈底
}

// 常量说明
void ConstDeclaration() {
	//match(Const);
	char *name;
	char *t;
	int num;
	// 常量定义
	do {
		next();
		name = current_id[Name];
		current_id[Type] = Defined;
		match(Id);
		match(Assign);
		num = token_val;
		match(Num);
	} while (token == ',');
	match(';');
}

// 变量说明
void VarDeclaration() {
	match(Var);
	// 变量定义
	while (token != ';') {
		current_id[Type] = Defined;
		match(Id);
		if (token == ',') {
			next();
		}
	}
	match(';');
}

int reverse_relop(int relop) {
	switch (relop) {
	case Eq:
		return Ne;
	case Ne:
		return Eq;
	case Lt:
		return Ge;
	case Gt:
		return Le;
	case Le:
		return Gt;
	case Ge:
		return Lt;
	}
}


// 语句
void Statement() {
	if (token == Id) {
		if (current_id[Type] != Defined) {
			printf("undefined symbol!");
			exit(-1);
		}
		// 赋值语句
		char* p = current_id[Name];
		next();
		if (token == Assign) {
			next();
			char* t = expression();

			char* t1 = TramformNumToName(++line);
			PrintOutRealName(t1);
			StoreText(" ");

			PrintOutRealName(p);
			StoreText(" := ");
			PrintOutRealName(t);
			StoreText("\n");
		}
	}
	else if (token == If) {
		next();
		// 取到判断表达式左边的符号
		char* x = expression();
		int relop = token;
		next();
		// 取到判断表达式右边的符号
		char* y = expression();
		next();

		// 中间代码行数
		char* t = TramformNumToName(++line);
		PrintOutRealName(t);
		StoreText(" ");

		// 依次输出 if x relop y goto next
		StoreText("if ");
		PrintOutRealName(x);
		StoreText(" ");
		// 将判断反转，因为我们的goto是条件跳转离开条件语句内部
		PrintOutRealOperator(reverse_relop(relop));
		StoreText(" ");
		PrintOutRealName(y);
		StoreText(" goto \n");

		// 记录下当前的text_p，因为 goto next 还没有生成 需要在Statement()之后生成
		// 得到后面的代码的行数，然后将这个行数插入到 goto next 的位置
		int old_text = text_p;
		// 语句部分
		Statement();
		InsertNumAt(line+1, old_text);
	}
	else if (token == While) {
		next();
		char* x = expression();
		int relop = token;
		next();
		char* y = expression();
		next();

		int temp = line + 1;

		char* t = TramformNumToName(++line);
		PrintOutRealName(t);
		StoreText(" ");

		StoreText("if ");
		PrintOutRealName(x);
		StoreText(" ");
		PrintOutRealOperator(reverse_relop(relop));
		StoreText(" ");
		PrintOutRealName(y);
		StoreText(" goto \n");
		int old_text = text_p;
		Statement();
		InsertNumAt(line+2, old_text);

		t = TramformNumToName(++line);
		PrintOutRealName(t);
		StoreText(" ");

		StoreText("goto ");
		t = TramformNumToName(temp);
		PrintOutRealName(t);
		StoreText("\n");
	}
	else if (token == Begin) {
		CompoundStatement();
	}
	else {
		expression();
		//match(';');
	}
}

// 复合语句
void CompoundStatement() {
	match(Begin);
	Statement();
	while (token == ';') {
		match(';');
		Statement();
	}
	match(End);
}

// 语句部分
void StatementPart() {
	if (token == Begin) {
		CompoundStatement();
	}
	else {
		Statement();
	}
}

// 程序首部
void ProgramHeader() {
	next(); // get next token
	match(Main);
	next(); // 标识符 
}

// 分程序
void SubProgram() {
	// 分程序首部
	// 分程序体
	if (token == Const) {
		ConstDeclaration();
	}
	if (token == Var) {
		VarDeclaration();
	}
	//StatementPart();
	Statement();
}


// 程序
void Program()
{
	ProgramHeader();
	SubProgram();
}

/*
	递归下降分析结束
*/



int Init() {
	if (!(symbols = malloc(poolsize))) {
		printf("could not malloc(%d) for symbol table\n", poolsize);
		return -1;
	}
	memset(symbols, 0, poolsize);

	// 读入源代码
	if (!(code = malloc(poolsize))) {
		printf("could not malloc(%d) for source area\n", poolsize);
		return -1;
	}

	// 内存区
	if (!(data = malloc(poolsize))) {
		printf("could not malloc(%d) for data\n", poolsize);
		return -1;
	}
	memset(data, 0, poolsize);
	current_data = data;

	// stack区
	if (!(symbol_stack = malloc(poolsize))) {
		printf("could not malloc(%d) for stack area\n", poolsize);
		return -1;
	}
	else {
		// 我这个栈就不去从高地址往低地址增长了，直接从低地址往高地址增长
		ssp = symbol_stack;
	}
	return 1;
}

void Destory() {
	if (symbols != NULL) {
		free(symbols);
	}
	if (code != NULL) {
		free(code);
	}

	if (symbol_stack != NULL) {
		free(symbol_stack);
	}
}

void GetCode() {
	int fd, i;
	char file[256]="test.txt";
	printf("请输入要编译的文件名：");
	if (!DEBUG) {
		scanf("%s", file);
	}
	src = code;

	if ((fd = open(file, 0)) < 0) {
		printf("could not open(%s)\n", file);
		return -1;
	}
	if ((i = read(fd, src, poolsize - 1)) <= 0) {
		printf("read() returned %d\n", i);
		return -1;
	}
	src[i] = 0; // add EOF character
	close(fd);
}

// 处理关键字
void DealKeyword() {
	src = keywords;
	int i;
	// add keywords to symbol table
	// 先处理入口地址 main
	next(); current_id[Type] = Main; current_id[Token] = Main;
	// 现在symbols第一个就是Main

	i = Begin;
	while (i <= Then) {
		next();
		current_id[Token] = i++; // 从1开始
	}
}

// 打印符号表
void PrintSymbolTable() {
	printf("\n");
	long long* p = symbols;
	char* name;
	while (p[Token]) {
		printf("Token: %lld", p[Token]);

		name = (char*)p[Name];

		printf("   Name:");
		while ((*name >= 'a' && *name <= 'z') || (*name >= 'A' && *name <= 'Z') || (*name >= '0' && *name <= '9') || (*name == '_')) {
			printf("%c", *name++);
		}
		printf(" Type: %lld", p[Type]);
		printf(" Class: %lld", p[Class]);
		printf(" Value: %lld", p[Value]);
		printf("\n");
		p = p + IdSize;
	}
}

// 打印数据区 主要是生成的字符串的保留位置
void ShowData() {
	printf("data: ");
	for (int i = 0; i < 150; i++) {
		printf("%c", data[i]);
	}
	printf("\n");
}

// 将结果写入文件
void WriteToFile() {
	FILE* file = fopen("output.txt", "w");

	// 检查文件是否成功打开
	if (file != NULL) {
		// 写入字符串到文件
		fprintf(file, text);

		// 关闭文件
		fclose(file);

		printf("文件写入成功！\n");
	}
	else {
		// 文件打开失败的情况
		printf("无法打开文件。\n");
	}
}

int main()
{

	if (Init() < 0) {
		Destory();
		return -1;
	}

	DealKeyword();

	GetCode();

	Program();

	if (DEBUG) {
		PrintSymbolTable();
		printf("%s", text);
	}
	Destory();
	WriteToFile();
	return 0;
}