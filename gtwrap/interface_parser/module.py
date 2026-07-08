"""
GTSAM Copyright 2010-2020, Georgia Tech Research Corporation,
Atlanta, Georgia 30332-0415
All Rights Reserved

See LICENSE for the license information

Rules and classes for parsing a module.

Author: Duy Nguyen Ta, Fan Jiang, Matthew Sklar, Varun Agrawal, and Frank Dellaert
"""

# pylint: disable=unnecessary-lambda, unused-import, expression-not-assigned, no-else-return, protected-access, too-few-public-methods, too-many-arguments

from pyparsing import (ParseBaseException, ParseResults, ZeroOrMore,  # type: ignore
                       cppStyleComment, stringEnd)

from .classes import Class
from .declaration import ForwardDeclaration, Include
from .enum import Enum
from .function import GlobalFunction
from .namespace import Namespace
from .template import TypedefTemplateInstantiation
from .variable import Variable


class InterfaceParseError(ValueError):
    """Error raised when an interface file cannot be parsed."""


def _format_parse_error(error: ParseBaseException,
                        source_name: str | None = None) -> str:
    source = source_name or "<string>"
    line_text = error.line or ""
    col = max(error.col, 1)
    message = (
        f"Failed to parse interface file {source} at "
        f"line {error.lineno}, column {error.col}: {error.msg}")
    if line_text:
        message += f"\n{line_text}\n{' ' * (col - 1)}^"
    return message


class Module:
    """
    Module is just a global namespace.

    E.g.
    ```
    namespace gtsam {
        ...
    }
    ```
    """

    rule = (
        ZeroOrMore(ForwardDeclaration.rule  #
                   ^ Include.rule  #
                   ^ Class.rule  #
                   ^ TypedefTemplateInstantiation.rule  #
                   ^ GlobalFunction.rule  #
                   ^ Enum.rule  #
                   ^ Variable.rule  #
                   ^ Namespace.rule  #
                   ).setParseAction(lambda t: Namespace('', t.asList())) +
        stringEnd)

    rule.ignore(cppStyleComment)

    @staticmethod
    def parseString(s: str, source_name: str | None = None) -> ParseResults:
        """Parse the source string and apply the rules."""
        try:
            return Module.rule.parseString(s)[0]
        except ParseBaseException as error:
            raise InterfaceParseError(
                _format_parse_error(error, source_name)) from error
