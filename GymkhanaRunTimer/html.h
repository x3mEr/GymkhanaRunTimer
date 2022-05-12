#pragma once

class CHtml
{
public:
  static const char* head;
  static const char* style;
  static const char* script;
  static const char* body;

  static String getPage();
  static String getHead();
  static String getScript();
  static String getBody();
};