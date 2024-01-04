grammar PL0;

// 语法规则
program : head block EOF; // 添加EOF，表示文件末尾

head : 'PROGRAM' identifier;

block : constDeclaration? varDeclaration? statement;

constDeclaration : 'CONST' constDefinition (',' constDefinition)* ';';

constDefinition : identifier ':=' unsignedInteger;

unsignedInteger : DIGIT (DIGIT)*; // 零怎么办

varDeclaration : 'VAR' identifier (',' identifier)* ';';

identifier : LETTER (LETTER | DIGIT)*;

compoundStatement : 'BEGIN' statement (';' statement)* 'END';

statement : assignmentStatement
          | ifStatement
          | whileStatement
          | compoundStatement
          | emptyStatement;

assignmentStatement : identifier ':=' expression;

expression : addOperator? term | expression addOperator term;

term : factor | term mulOperator factor;

factor : identifier | unsignedInteger | '(' expression ')';

addOperator : '+' | '-';

mulOperator : '*' | '/';

ifStatement : 'IF' condition 'THEN' statement;

whileStatement : 'WHILE' condition 'DO' statement;

condition : expression relationalOperator expression;

relationalOperator : '=' | '<>' | '<' | '<=' | '>' | '>=';

// 词法规则
LETTER : [a-z];
DIGIT : [0-9];
WS : [ \t\r\n]+ -> skip;
COMMENT : '//' ~[\r\n]* -> skip;

// 空语句
emptyStatement : ;