// Generated from D:/CodeCPy/JavaProjects/Compiler/src/antlr/PL0.g4 by ANTLR 4.13.1
package PL0;
import org.antlr.v4.runtime.tree.ParseTreeVisitor;

/**
 * This interface defines a complete generic visitor for a parse tree produced
 * by {@link PL0Parser}.
 *
 * @param <T> The return type of the visit operation. Use {@link Void} for
 * operations with no return type.
 */
public interface PL0Visitor<T> extends ParseTreeVisitor<T> {
	/**
	 * Visit a parse tree produced by {@link PL0Parser#program}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitProgram(PL0Parser.ProgramContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#head}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitHead(PL0Parser.HeadContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#block}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitBlock(PL0Parser.BlockContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#constDeclaration}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitConstDeclaration(PL0Parser.ConstDeclarationContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#constDefinition}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitConstDefinition(PL0Parser.ConstDefinitionContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#unsignedInteger}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitUnsignedInteger(PL0Parser.UnsignedIntegerContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#varDeclaration}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitVarDeclaration(PL0Parser.VarDeclarationContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#identifier}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitIdentifier(PL0Parser.IdentifierContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#compoundStatement}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitCompoundStatement(PL0Parser.CompoundStatementContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#statement}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitStatement(PL0Parser.StatementContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#assignmentStatement}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAssignmentStatement(PL0Parser.AssignmentStatementContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#expression}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitExpression(PL0Parser.ExpressionContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#term}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTerm(PL0Parser.TermContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#factor}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFactor(PL0Parser.FactorContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#addOperator}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAddOperator(PL0Parser.AddOperatorContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#mulOperator}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitMulOperator(PL0Parser.MulOperatorContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#ifStatement}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitIfStatement(PL0Parser.IfStatementContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#whileStatement}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitWhileStatement(PL0Parser.WhileStatementContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#condition}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitCondition(PL0Parser.ConditionContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#relationalOperator}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitRelationalOperator(PL0Parser.RelationalOperatorContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#emptyStatement}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitEmptyStatement(PL0Parser.EmptyStatementContext ctx);
}