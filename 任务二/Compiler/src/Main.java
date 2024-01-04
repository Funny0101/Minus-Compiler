import antlr.PL0.PL0BaseVisitorImpl;
import PL0.PL0Lexer;
import PL0.PL0Parser;
import org.antlr.v4.runtime.CharStreams;
import org.antlr.v4.runtime.CommonTokenStream;

import java.io.PrintWriter;


public class Main {
    public static void main(String[] args) {
        try {
            String filePath = "example.txt"; // 为了方便，固定输入
            PL0Lexer lexer = new PL0Lexer(CharStreams.fromFileName(filePath));
            CommonTokenStream tokens = new CommonTokenStream(lexer);
            PL0Parser parser = new PL0Parser(tokens);

            parser.removeErrorListeners(); // 移除原有的错误检测
            parser.addErrorListener(new CustomErrorListener());

            parser.setBuildParseTree(true);
            PL0Parser.ProgramContext tree = parser.program();
            PL0BaseVisitorImpl visitor = new PL0BaseVisitorImpl();

            // 检查是否存在语法错误
            if (CustomErrorListener.syntaxErrorOccurred) {
                System.out.println("Syntax errors occurred. Aborting code generation.");
            } else {
                visitor.visit(tree); // 遍历AST

                PrintWriter writer = new PrintWriter("output.txt");
                writer.println(visitor.getIntermediateCode());
                System.out.println("Intermediate code has been written to output.txt.");
                writer.close();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}