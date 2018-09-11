#include "sbzlibraryscope.h"

SBZLibraryScope *SBZLibraryScope::shared()
{
  static SBZLibraryScope instance;
  return &instance;
}

SBZLibraryScope::SBZLibraryScope()
  : _dll(nullptr)
  , _library(nullptr)
{
  _dll = LoadLibraryA("SBZWhakman.dll");
  CreateLibraryFunc create_library = reinterpret_cast<CreateLibraryFunc>(GetProcAddress(_dll, "CreateLibrary"));
  _library = create_library();
}

SBZLibraryScope::~SBZLibraryScope() {
  FreeLibrary(_dll);
}

ISBZLibrary * SBZLibraryScope::library() const {
  return _library;
}
