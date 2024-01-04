package antlr.PL0;

import PL0.PL0BaseVisitor;
import PL0.PL0Parser;

public class PL0BaseVisitorImpl extends PL0BaseVisitor<String> {

    private StringBuilder intermediateCode;
    private int tempVariableCount;

    private int label;

    public PL0BaseVisitorImpl() {
        intermediateCode = new StringBuilder();
        tempVariableCount = 0;
        label = 100;
    }

    private String newTempVar() {
        return "T" + tempVariableCount++;
    }

    public int getLabel() {
        return label;
    }

    public void increaseLabel() {
        label += 1;
    }

    public String getIntermediateCode() {
        return intermediateCode.toString();
    }

    @Override
    public String visitProgram(PL0Parser.ProgramContext ctx) {
        intermediateCode.append("START\n");
        visitChildren(ctx);
        intermediateCode.append("STOP\n");
        return null;
    }

    @Override
    public String visitAssignmentStatement(PL0Parser.AssignmentStatementContext ctx) {
        String identifier = ctx.identifier().getText();
        String value = visit(ctx.expression());
        intermediateCode.append(getLabel()).append("  ").append(identifier)
                .append(" := ").append(value).append("\n");
        increaseLabel();
        return null;
    }

    @Override
    public String visitExpression(PL0Parser.ExpressionContext ctx) {
        String term = visit(ctx.term());
        if (ctx.addOperator() == null) {
            return term;
        } else {
            String addOp = ctx.addOperator().getText();
            String tempVariable = newTempVar();
            if (ctx.expression() == null) {
                intermediateCode.append(getLabel()).append("  ").append(tempVariable)
                        .append(" := ").append(addOp).append(term).append("\n");
                increaseLabel();
            } else {
                String expression = visit(ctx.expression());
                intermediateCode.append(getLabel()).append("  ").append(tempVariable)
                        .append(" := ").append(expression).append(addOp).append(term).append("\n");
                increaseLabel();
            }
            return tempVariable;
        }
    }

    @Override
    public String visitTerm(PL0Parser.TermContext ctx) {
        String factor = visit(ctx.factor());
        if (ctx.mulOperator() == null) {
            return factor;
        } else {
            String tempVariable = newTempVar();
            String term = visitTerm(ctx.term());
            String mulOperator = ctx.mulOperator().getText();
            intermediateCode.append(getLabel()).append("  ").append(tempVariable)
                    .append(" := ").append(term).append(mulOperator).append(factor).append("\n");
            increaseLabel();
            return tempVariable;
        }
    }

    @Override
    public String visitFactor(PL0Parser.FactorContext ctx) {
        if (ctx.expression() != null) {
            return visit(ctx.expression());
        } else {
            return ctx.getText();
        }
    }

    @Override
    public String visitIfStatement(PL0Parser.IfStatementContext ctx) {
        String condition = visit(ctx.condition());

        int currentLabel = getLabel(); // 获取当前标签
        // 先添加占位的 GOTO 语句，跳转位置先不确定
        intermediateCode.append(currentLabel).append("  ").append("IF ")
                .append(reverseRelop(condition)).append(" GOTO ").append("TBD").append("\n");
        increaseLabel();

        visit(ctx.statement());
        int endLabel = getLabel(); // 获取语句体结束的标签
        // 在之前的占位 GOTO 语句中添加正确的跳转位置，并只在第一次生成 NOOP 语句
        intermediateCode.replace(intermediateCode.indexOf("TBD"),
                intermediateCode.indexOf("TBD") + "TBD".length(), String.valueOf(endLabel));
        return null;
    }

    @Override
    public String visitWhileStatement(PL0Parser.WhileStatementContext ctx) {
        String condition = visit(ctx.condition());

        int startLabel = getLabel(); // 获取循环开始标签
        intermediateCode.append(startLabel).append("  ").append("IF ")
                .append(reverseRelop(condition)).append(" GOTO ").append("TBD").append("\n");
        increaseLabel();
        visit(ctx.statement());

        // 在之前的占位 IF GOTO 语句中添加正确的跳转位置，并添加 GOTO 语句
        intermediateCode.append(getLabel()).append("  ").append("GOTO ").append(startLabel).append("\n");
        increaseLabel();
        int endLabel = getLabel(); // 获取循环结束标签
        intermediateCode.replace(intermediateCode.indexOf("TBD"),
                intermediateCode.indexOf("TBD") + "TBD".length(), String.valueOf(endLabel));
        return null;
    }

    @Override
    public String visitCondition(PL0Parser.ConditionContext ctx) {
        String left = visit(ctx.expression(0));
        String relationOp = ctx.relationalOperator().getText();
        String right = visit(ctx.expression(1));
        return left + ' ' + relationOp + ' ' + right;
    }

    private String reverseRelop(String condition) {
        String[] temp = condition.split(" ");
        if (temp.length != 3) {
            // 处理异常情况，条件表达式的格式不符合预期
            return condition;
        }
        String relop = temp[1];
        // 简化反转关系运算符的过程
        switch (relop) {
            case "=" -> relop = "<>";
            case "<>" -> relop = "=";
            case "<" -> relop = ">=";
            case "<=" -> relop = ">";
            case ">" -> relop = "<=";
            case ">=" -> relop = "<";
            default -> {
                return condition;// 如果关系运算符不是预期的值，保持原样
            }
        }
        // 返回反转后的条件表达式
        return String.join(" ", temp[0], relop, temp[2]);
    }
}

