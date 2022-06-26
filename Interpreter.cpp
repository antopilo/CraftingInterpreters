#include "Interpreter.h"

#include "RuntimeError.h"
#include "Lox.h"

#include <iostream>

void Interpreter::Interpret(std::vector<StmtPtr> statements)
{
	try
	{
		for (auto& statement : statements)
		{
			Execute(statement);
		}
		
	} catch (RuntimeError error)
	{
		Lox::RuntimeErr(error);
	}
}

std::any Interpreter::VisitLiteralExpr(const Literal& expr)
{
	return expr.GetLiteral();
}

std::any Interpreter::VisitGroupingExpr(const Grouping& expr)
{
	return Evaluate(expr.GetExpr());
}

std::any Interpreter::VisitUnaryExpr(const Unary& expr)
{
	std::any right = Evaluate(expr.GetRight());

	switch (expr.GetOperator().GetType())
	{
		case TokenType::BANG:
		{
			return !IsTruthy(right);
		}
		case TokenType::MINUS:
		{
			CheckNumberOperand(expr.GetOperator(), right);
			return -std::any_cast<double>(right);
		}
	}

	// Unreachable.
	return nullptr;
}

std::any Interpreter::VisitBinaryExpr(const Binary& expr)
{
	std::any left = Evaluate(expr.GetLeftExpr());
	std::any right = Evaluate(expr.GetRightExpr());

	switch (expr.GetOp().GetType())
	{
		case TokenType::BANG_EQUAL:
		{
			return !IsEqual(left, right);
		}
		case TokenType::EQUAL_EQUAL:
		{
			return IsEqual(left, right);
		}
		case TokenType::GREATER:
		{
			CheckNumberOperands(expr.GetOp(), left, right);
			return std::any_cast<double>(left) > std::any_cast<double>(right);
		}
		case TokenType::GREATER_EQUAL:
		{
			CheckNumberOperands(expr.GetOp(), left, right);
			return std::any_cast<double>(left) >= std::any_cast<double>(right);
		}
		case TokenType::LESS:
		{
			CheckNumberOperands(expr.GetOp(), left, right);
			return std::any_cast<double>(left) < std::any_cast<double>(right);
		}
		case TokenType::LESS_EQUAL:
		{
			CheckNumberOperands(expr.GetOp(), left, right);
			return std::any_cast<double>(left) <= std::any_cast<double>(right);
		}
		case TokenType::MINUS:
		{
			CheckNumberOperands(expr.GetOp(), left, right);
			return std::any_cast<double>(left) - std::any_cast<double>(right);
		}
		case TokenType::PLUS:
		{
			if (left.type() == typeid(double) && right.type() == typeid(double))
			{
				return std::any_cast<double>(left) + std::any_cast<double>(right);
			}

			bool leftIsString = left.type() == typeid(std::string);
			bool rightIsString = right.type() == typeid(std::string);
			if (leftIsString && rightIsString)
			{
				return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
			}
			else
			{
				std::string leftString = leftIsString ? std::any_cast<std::string>(left) : Stringify(left);
				std::string rightString = rightIsString ? std::any_cast<std::string>(right) : Stringify(right);
				return leftString + rightString;
			}

			throw RuntimeError(expr.GetOp(), "Operands must be two numbers or two strings.");
		}
		case TokenType::SLASH:
		{
			CheckNumberOperands(expr.GetOp(), left, right);
			double denomiator = std::any_cast<double>(right);
			if (denomiator == 0.0)
			{
				throw RuntimeError(expr.GetOp(), "Division by 0.");
			}

			return std::any_cast<double>(left) / std::any_cast<double>(right);
		}
		case TokenType::STAR:
		{
			CheckNumberOperands(expr.GetOp(), left, right);
			return std::any_cast<double>(left) * std::any_cast<double>(right);
		}
	}

	return nullptr;
}

std::any Interpreter::visitExpressionStmt(const ExpressionStmt& stmt)
{
	Evaluate(stmt.GetExpression());
	return nullptr;
}

std::any Interpreter::visitPrintStmt(const PrintStmt& stmt)
{
	std::any value = Evaluate(stmt.GetExpression());
	std::cout << Stringify(value) << std::endl;
	return nullptr;
}

void Interpreter::Execute(StmtPtr stmt)
{
	stmt->Accept(*this);
}

std::string Interpreter::Stringify(std::any object)
{
	if (!object.has_value())
	{
		return "nil";
	}

	if (object.type() == typeid(bool))
	{
		return std::any_cast<bool>(object) ? "true" : "false";
	}

	if (object.type() == typeid(double))
	{
		double number = std::any_cast<double>(object);
		if (std::trunc(number) == number) 
		{
			return std::to_string((int)number);
		}

		return std::to_string(number);
	}

	if (object.type() == typeid(std::string))
	{
		return std::any_cast<std::string>(object);
	}

	return "";

}

std::any Interpreter::Evaluate(ExprPtr expr)
{
	return expr->Accept(reinterpret_cast<ExprVisitor<std::any>&>(*this));
}

bool Interpreter::IsEqual(std::any left, std::any right)
{
	if (!left.has_value() && !right.has_value())
	{
		return true;
	}

	if (!left.has_value())
	{
		return false;
	}

	bool leftIsBool = left.type() == typeid(bool);
	bool leftIsDouble = left.type() == typeid(double);
	if (left.type() != right.type()) 
	{
		bool rightIsBool = right.type() == typeid(bool);
		bool rightIsDouble = right.type() == typeid(double);

		if (leftIsBool && rightIsDouble)
		{
			bool leftB = std::any_cast<bool>(left);
			double leftDouble = leftB ? 1.0 : 0.0;
			return leftDouble == std::any_cast<double>(right);
		}

		if (leftIsDouble && rightIsBool)
		{
			bool rightb = std::any_cast<bool>(right);
			double rightDouble = rightb ? 1.0 : 0.0;
			return rightDouble == std::any_cast<double>(left);
		}

		return false;
	}

	if (leftIsBool)
	{
		return std::any_cast<bool>(left) == std::any_cast<bool>(right);
	}

	if (left.type() == typeid(double)) 
	{
		return std::any_cast<double>(left) == std::any_cast<double>(right);
	}

	// bool and doubles


	if (left.type() == typeid(std::string)) 
	{
		return std::any_cast<std::string>(left) == std::any_cast<std::string>(right);
	}

	return false;
}

void Interpreter::CheckNumberOperand(Token op, std::any operand)
{
	if (operand.type() == typeid(double))
	{
		return;
	}

	throw RuntimeError(op, "Operand must be a number.");
}

void Interpreter::CheckNumberOperands(Token op, std::any left, std::any right)
{
	if (left.type() == typeid(double) && right.type() == typeid(double))
	{
		return;
	}

	throw RuntimeError(op, "Operands must be a numbers.");
}

bool Interpreter::IsTruthy(std::any object)
{
	if (!object.has_value())
	{
		return false;
	}

	if (object.type() == typeid(false))
	{
		return std::any_cast<bool>(object);
	}

	return true;
}