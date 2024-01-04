#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG 0

int poolsize = 256 * 1024;
char keywords[] = "PROGRAM BEGIN END CONST VAR WHILE DO IF THEN";
char* src; // ָ��Դ����

char text[9096] = "";
int text_p = 0;

// ����������ŵ�����
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

// ����ջ
long long* symbol_stack, * ssp;
// ���ջ
int operator_stack[100], osp=0;

// ����ͳ��
int line = 0;

void PrintSymbolTable();
void CompoundStatement();

char* NameGenerator() {
	char* p;
	p = current_data;
	// ����һ���� t_1 t_2 t_3 ����������
	NameCounter++;
	*current_data = 't';
	*++current_data = '_';
	// �� NameCounter ת�����ַ��� ������� current_data
	// ��ȡ NameCounter ��λ��
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
	// �� NameCounter ת�����ַ��� ������� current_data
	// ��ȡ NameCounter ��λ��
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
	// ��ȡnum��λ��
	int temp = num;
	int weight = 1;
	int count = 1;
	while (temp != 0) {
		temp /= 10;
		weight++;
		count *= 10;
	}
	// ��text��pos��ʼ�����ƶ�countλ
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

	// ������
	if (src == '\0' || *src == EOF) {
		token = 0;
		return;
	}
	while (token = *src) {
		++src;
		if (token == '\n') {

		}
		else if (token >= 'a' && token <= 'z' || token >= 'A' && token <= 'Z' || token == '_') {
			begin_pos = src - 1; // ������������ƵĿ�ʼ�ĵط�
			hash = token;

			// ������������
			while ((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_')) {
				hash = hash * 147 + *src;
				src++;
			}


			current_id = symbols; // ָ����ű�Ŀ�ʼ
			while (current_id[Token]) {
				// �����ǲ����Ѿ�������
				if (current_id[Hash] == hash) {
					token = current_id[Token];
					return;
				}
				current_id = current_id + IdSize; // ָ����һ������
			}

			// ������˵��û���ҵ�����ô�����µķ��� 
			// char * ��8�ֽڣ�ת long long
			current_id[Hash] = hash;
			current_id[Name] = (long long)begin_pos; // ���ƵĿ�ʼ�ĵط� ֱ�Ӿ����Ǹ������ĵ�ַ
			token = current_id[Token] = Id; // token��ֵ
			return;
		}
		else if (token >= '0' && token <= '9') {
			// ֻ֧����ʮ����
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
	�ݹ��½�������ʼ
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
		// ���
		else if (token == Add || token == Sub || token == Mul || token == Div || token == Right) {
			if (osp == 0) {
				operator_stack[osp] = token;
				osp++;
			}
			else if (token <= operator_stack[osp - 1]) {
				// ˵��ջ�������ȼ��Ƚϸ�
				// ��ջ����ʼ����
				while (osp != 0 && token <= operator_stack[osp - 1]) {
					if (operator_stack[osp - 1] == Left) {
						if (token == Right) {
							// ˵��������
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
					// ��������֮�󣬽�����ŵ�ջ��
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

	// ������ջ����Ķ���
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
		// ��������֮�󣬽�����ŵ�ջ��
		*(ssp - 2) = (long long)t;
		ssp -= 1;
		osp -= 1;
	}
	// ���ջ
	ssp = symbol_stack;
	osp = 0;
	// printf("%c", *(char*)*symbol_stack);
	return *symbol_stack; // ��Ϊ���һ����õķ��ſ϶������ջ��
}

// ����˵��
void ConstDeclaration() {
	//match(Const);
	char *name;
	char *t;
	int num;
	// ��������
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

// ����˵��
void VarDeclaration() {
	match(Var);
	// ��������
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


// ���
void Statement() {
	if (token == Id) {
		if (current_id[Type] != Defined) {
			printf("undefined symbol!");
			exit(-1);
		}
		// ��ֵ���
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
		// ȡ���жϱ��ʽ��ߵķ���
		char* x = expression();
		int relop = token;
		next();
		// ȡ���жϱ��ʽ�ұߵķ���
		char* y = expression();
		next();

		// �м��������
		char* t = TramformNumToName(++line);
		PrintOutRealName(t);
		StoreText(" ");

		// ������� if x relop y goto next
		StoreText("if ");
		PrintOutRealName(x);
		StoreText(" ");
		// ���жϷ�ת����Ϊ���ǵ�goto��������ת�뿪��������ڲ�
		PrintOutRealOperator(reverse_relop(relop));
		StoreText(" ");
		PrintOutRealName(y);
		StoreText(" goto \n");

		// ��¼�µ�ǰ��text_p����Ϊ goto next ��û������ ��Ҫ��Statement()֮������
		// �õ�����Ĵ����������Ȼ������������뵽 goto next ��λ��
		int old_text = text_p;
		// ��䲿��
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

// �������
void CompoundStatement() {
	match(Begin);
	Statement();
	while (token == ';') {
		match(';');
		Statement();
	}
	match(End);
}

// ��䲿��
void StatementPart() {
	if (token == Begin) {
		CompoundStatement();
	}
	else {
		Statement();
	}
}

// �����ײ�
void ProgramHeader() {
	next(); // get next token
	match(Main);
	next(); // ��ʶ�� 
}

// �ֳ���
void SubProgram() {
	// �ֳ����ײ�
	// �ֳ�����
	if (token == Const) {
		ConstDeclaration();
	}
	if (token == Var) {
		VarDeclaration();
	}
	//StatementPart();
	Statement();
}


// ����
void Program()
{
	ProgramHeader();
	SubProgram();
}

/*
	�ݹ��½���������
*/



int Init() {
	if (!(symbols = malloc(poolsize))) {
		printf("could not malloc(%d) for symbol table\n", poolsize);
		return -1;
	}
	memset(symbols, 0, poolsize);

	// ����Դ����
	if (!(code = malloc(poolsize))) {
		printf("could not malloc(%d) for source area\n", poolsize);
		return -1;
	}

	// �ڴ���
	if (!(data = malloc(poolsize))) {
		printf("could not malloc(%d) for data\n", poolsize);
		return -1;
	}
	memset(data, 0, poolsize);
	current_data = data;

	// stack��
	if (!(symbol_stack = malloc(poolsize))) {
		printf("could not malloc(%d) for stack area\n", poolsize);
		return -1;
	}
	else {
		// �����ջ�Ͳ�ȥ�Ӹߵ�ַ���͵�ַ�����ˣ�ֱ�Ӵӵ͵�ַ���ߵ�ַ����
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
	printf("������Ҫ������ļ�����");
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

// ����ؼ���
void DealKeyword() {
	src = keywords;
	int i;
	// add keywords to symbol table
	// �ȴ�����ڵ�ַ main
	next(); current_id[Type] = Main; current_id[Token] = Main;
	// ����symbols��һ������Main

	i = Begin;
	while (i <= Then) {
		next();
		current_id[Token] = i++; // ��1��ʼ
	}
}

// ��ӡ���ű�
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

// ��ӡ������ ��Ҫ�����ɵ��ַ����ı���λ��
void ShowData() {
	printf("data: ");
	for (int i = 0; i < 150; i++) {
		printf("%c", data[i]);
	}
	printf("\n");
}

// �����д���ļ�
void WriteToFile() {
	FILE* file = fopen("output.txt", "w");

	// ����ļ��Ƿ�ɹ���
	if (file != NULL) {
		// д���ַ������ļ�
		fprintf(file, text);

		// �ر��ļ�
		fclose(file);

		printf("�ļ�д��ɹ���\n");
	}
	else {
		// �ļ���ʧ�ܵ����
		printf("�޷����ļ���\n");
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