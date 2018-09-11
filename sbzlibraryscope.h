#pragma once
#include <Windows.h>
#include "sbzwhakman.h"

class SBZLibraryScope {
public:
  static SBZLibraryScope *shared();
  typedef ISBZLibrary*(*CreateLibraryFunc)();

  SBZLibraryScope();
  ~SBZLibraryScope();
  ISBZLibrary	*library() const;
private:
  HMODULE			_dll;
  ISBZLibrary*	_library;
};