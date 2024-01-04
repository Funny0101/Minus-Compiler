// Generated from D:/CodeCPy/JavaProjects/Compiler/src/antlr/PL0.g4 by ANTLR 4.13.1
package PL0;
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link PL0Parser}.
 */
public interface PL0Listener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link PL0Parser#program}.
	 * @param ctx the parse tree
	 */
	void enterProgram(PL0Parser.ProgramContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#program}.
	 * @param ctx the parse tree
	 */
	void exitProgram(PL0Parser.ProgramContext ctx);
	/**
	 * Enter a parse tree produced by {@link PL0Parser#head}.
	 * @param ctx the parse tree
	 */
	void enterHead(PL0Parser.HeadContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#head}.
	 * @param ctx the parse tree
	 */
	void exitHead(PL0Parser.HeadContext ctx);
	/**
	 * Enter a parse tree produced by {@link PL0Parser#block}.
	 * @param ctx the parse tree
	 */
	void enterBlock(PL0Parser.BlockContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#block}.
	 * @param ctx the parse tree
	 */
	void exitBlock(PL0Parser.BlockContext ctx);
	/**
	 * Enter a parse tree produced by {@link PL0Parser#constDeclaration}.
	 * @param ctx the parse tree
	 */
	void enterConstDeclaration(PL0Parser.ConstDeclarationContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#constDeclaration}.
	 * @param ctx the parse tree
	 */
	void exitConstDeclaration(PL0Parser.ConstDeclarationContext ctx);
	/**
	 * Enter a parse tree produced by {@link PL0Parser#constDefinition}.
	 * @param ctx the parse tree
	 */
	void enterConstDefinition(PL0Parser.ConstDefinitionContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#constDefinition}.
	 * @param ctx the parse tree
	 */
	void exitConstDefinition(PL0Parser.ConstDefinitionContext ctx);
	/**
	 * Enter a parse tree produced by {@link PL0Parser#unsignedInteger}.
	 * @param ctx the parse tree
	 */
	void enterUnsignedInteger(PL0Parser.UnsignedIntegerContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#unsignedInteger}.
	 * @param ctx the parse tree
	 */
	void exitUnsignedInteger(PL0Parser.UnsignedIntegerContext ctx);
	/**
	 * Enter a parse tree produced by {@link PL0Parser#varDeclaration}.
	 * @param ctx the parse tree
	 */
	void enterVarDeclaration(PL0Parser.VarDeclarationContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#varDeclaration}.
	 * @param ctx the parse tree
	 */
	void exitVarDeclaration(PL0Parser.VarDeclarationContext ctx);
	/**
	 * Enter a parse tree produced by {@link PL0Parser#identifier}.
	 * @param ctx the parse tree
	 */
	void enterIdentifier(PL0Parser.IdentifierContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#identifier}.
	 * @param ctx the parse tree
	 */
	void exitIdentifier(PL0Parser.IdentifierContext ctx);
	/**
	 * Enter a parse tree produced by {@link PL0Parser#compoundStatement}.
	 * @param ctx the parse tree
	 */
	void enterCompoundStatement(PL0Parser.CompoundStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#compoundStatement}.
	 * @param ctx the parse tree
	 */
	void exitCompoundStatement(PL0Parser.CompoundStatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link PL0Parser#statement}.
	 * @param ctx the parse tree
	 */
	void enterStatement(PL0Parser.StatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#statement}.
	 * @param ctx the parse tree
	 */
	void exitStatement(PL0Parser.StatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link PL0Parser#assignmentStatement}.
	 * @param ctx the parse tree
	 */
	void enterAssignmentStatement(PL0Parser.AssignmentStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#assignmentStatement}.
	 * @param ctx the parse tree
	 */
	void exitAssignmentStatement(PL0Parser.AssignmentStatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link PL0Parser#expression}.
	 * @param ctx the parse tree
	 */
	void enterExpression(PL0Parser.ExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#expression}.
	 * @param ctx the parse tree
	 */
	void exitExpression(PL0Parser.ExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link PL0Parser#term}.
	 * @param ctx the parse tree
	 */
	void enterTerm(PL0Parser.TermContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#term}.
	 * @param ctx the parse tree
	 */
	void exitTerm(PL0Parser.TermContext ctx);
	/**
	 * Enter a parse tree produced by {@link PL0Parser#factor}.
	 * @param ctx the parse tree
	 */
	void enterFactor(PL0Parser.FactorContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#factor}.
	 * @param ctx the parse tree
	 */
	void exitFactor(PL0Parser.FactorContext ctx);
	/**
	 * Enter a parse tree produced by {@link PL0Parser#addOperator}.
	 * @param ctx the parse tree
	 */
	void enterAddOperator(PL0Parser.AddOperatorContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#addOperator}.
	 * @param ctx the parse tree
	 */
	void exitAddOperator(PL0Parser.AddOperatorContext ctx);
	/**
	 * Enter a parse tree produced by {@link PL0Parser#mulOperator}.
	 * @param ctx the parse tree
	 */
	void enterMulOperator(PL0Parser.MulOperatorContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#mulOperator}.
	 * @param ctx the parse tree
	 */
	void exitMulOperator(PL0Parser.MulOperatorContext ctx);
	/**
	 * Enter a parse tree produced by {@link PL0Parser#ifStatement}.
	 * @param ctx the parse tree
	 */
	void enterIfStatement(PL0Parser.IfStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#ifStatement}.
	 * @param ctx the parse tree
	 */
	void exitIfStatement(PL0Parser.IfStatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link PL0Parser#whileStatement}.
	 * @param ctx the parse tree
	 */
	void enterWhileStatement(PL0Parser.WhileStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#whileStatement}.
	 * @param ctx the parse tree
	 */
	void exitWhileStatement(PL0Parser.WhileStatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link PL0Parser#condition}.
	 * @param ctx the parse tree
	 */
	void enterCondition(PL0Parser.ConditionContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#condition}.
	 * @param ctx the parse tree
	 */
	void exitCondition(PL0Parser.ConditionContext ctx);
	/**
	 * Enter a parse tree produced by {@link PL0Parser#relationalOperator}.
	 * @param ctx the parse tree
	 */
	void enterRelationalOperator(PL0Parser.RelationalOperatorContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#relationalOperator}.
	 * @param ctx the parse tree
	 */
	void exitRelationalOperator(PL0Parser.RelationalOperatorContext ctx);
	/**
	 * Enter a parse tree produced by {@link PL0Parser#emptyStatement}.
	 * @param ctx the parse tree
	 */
	void enterEmptyStatement(PL0Parser.EmptyStatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link PL0Parser#emptyStatement}.
	 * @param ctx the parse tree
	 */
	void exitEmptyStatement(PL0Parser.EmptyStatementContext ctx);
}