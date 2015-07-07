# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.5
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from __future__ import absolute_import





from sys import version_info
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_logging', [dirname(__file__)])
        except ImportError:
            import _logging
            return _logging
        if fp is not None:
            try:
                _mod = imp.load_module('_logging', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _logging = swig_import_helper()
    del swig_import_helper
else:
    import _logging
del version_info
try:
    _swig_property = property
except NameError:
    pass  # Python < 2.2 doesn't have 'property'.


def _swig_setattr_nondynamic(self, class_type, name, value, static=1):
    if (name == "thisown"):
        return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name, None)
    if method:
        return method(self, value)
    if (not static):
        if _newclass:
            object.__setattr__(self, name, value)
        else:
            self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)


def _swig_setattr(self, class_type, name, value):
    return _swig_setattr_nondynamic(self, class_type, name, value, 0)


def _swig_getattr_nondynamic(self, class_type, name, static=1):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    if (not static):
        return object.__getattr__(self, name)
    else:
        raise AttributeError(name)

def _swig_getattr(self, class_type, name):
    return _swig_getattr_nondynamic(self, class_type, name, 0)


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object:
        pass
    _newclass = 0


class Formatter(_object):
    """Proxy of C++ logging::Formatter class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Formatter, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Formatter, name)

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _logging.delete_Formatter
    __del__ = lambda self: None

    def format(self, record, os):
        """format(Formatter self, LogRecord const * record, io::OutputStream & os)"""
        return _logging.Formatter_format(self, record, os)


    def getPrologue(self):
        """getPrologue(Formatter self) -> std::string"""
        return _logging.Formatter_getPrologue(self)


    def getEpilogue(self):
        """getEpilogue(Formatter self) -> std::string"""
        return _logging.Formatter_getEpilogue(self)

Formatter_swigregister = _logging.Formatter_swigregister
Formatter_swigregister(Formatter)

class StandardFormatter(Formatter):
    """Proxy of C++ logging::StandardFormatter class"""
    __swig_setmethods__ = {}
    for _s in [Formatter]:
        __swig_setmethods__.update(getattr(_s, '__swig_setmethods__', {}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, StandardFormatter, name, value)
    __swig_getmethods__ = {}
    for _s in [Formatter]:
        __swig_getmethods__.update(getattr(_s, '__swig_getmethods__', {}))
    __getattr__ = lambda self, name: _swig_getattr(self, StandardFormatter, name)
    __repr__ = _swig_repr

    def __init__(self, *args):
        """
        __init__(logging::StandardFormatter self) -> StandardFormatter
        __init__(logging::StandardFormatter self, std::string const & fmt, std::string const & prologue, std::string const & epilogue) -> StandardFormatter
        __init__(logging::StandardFormatter self, std::string const & fmt, std::string const & prologue) -> StandardFormatter
        __init__(logging::StandardFormatter self, std::string const & fmt) -> StandardFormatter
        """
        this = _logging.new_StandardFormatter(*args)
        try:
            self.this.append(this)
        except:
            self.this = this
    __swig_destroy__ = _logging.delete_StandardFormatter
    __del__ = lambda self: None

    def format(self, record, os):
        """format(StandardFormatter self, LogRecord const * record, io::OutputStream & os)"""
        return _logging.StandardFormatter_format(self, record, os)

StandardFormatter_swigregister = _logging.StandardFormatter_swigregister
StandardFormatter_swigregister(StandardFormatter)
cvar = _logging.cvar
StandardFormatter.DEFAULT_FORMAT = _logging.cvar.StandardFormatter_DEFAULT_FORMAT

class Filterer(_object):
    """Proxy of C++ logging::Filterer class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Filterer, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Filterer, name)
    __repr__ = _swig_repr

    def __init__(self):
        """__init__(logging::Filterer self) -> Filterer"""
        this = _logging.new_Filterer()
        try:
            self.this.append(this)
        except:
            self.this = this
    __swig_destroy__ = _logging.delete_Filterer
    __del__ = lambda self: None

    def addFilter(self, filter):
        """addFilter(Filterer self, Filter * filter)"""
        return _logging.Filterer_addFilter(self, filter)


    def filter(self, record):
        """filter(Filterer self, LogRecord const * record) -> bool"""
        return _logging.Filterer_filter(self, record)


    def removeFilter(self, filter):
        """removeFilter(Filterer self, Filter * filter)"""
        return _logging.Filterer_removeFilter(self, filter)

Filterer_swigregister = _logging.Filterer_swigregister
Filterer_swigregister(Filterer)

class Handler(Filterer):
    """Proxy of C++ logging::Handler class"""
    __swig_setmethods__ = {}
    for _s in [Filterer]:
        __swig_setmethods__.update(getattr(_s, '__swig_setmethods__', {}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, Handler, name, value)
    __swig_getmethods__ = {}
    for _s in [Filterer]:
        __swig_getmethods__.update(getattr(_s, '__swig_getmethods__', {}))
    __getattr__ = lambda self, name: _swig_getattr(self, Handler, name)

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _logging.delete_Handler
    __del__ = lambda self: None

    def setLevel(self, level):
        """setLevel(Handler self, LogLevel level)"""
        return _logging.Handler_setLevel(self, level)


    def getLevel(self):
        """getLevel(Handler self) -> LogLevel"""
        return _logging.Handler_getLevel(self)


    def handle(self, record):
        """handle(Handler self, LogRecord const * record) -> bool"""
        return _logging.Handler_handle(self, record)


    def close(self):
        """close(Handler self)"""
        return _logging.Handler_close(self)


    def setFormatter(self, formatter):
        """setFormatter(Handler self, Formatter formatter)"""
        return _logging.Handler_setFormatter(self, formatter)

Handler_swigregister = _logging.Handler_swigregister
Handler_swigregister(Handler)

class StreamHandler(Handler):
    """Proxy of C++ logging::StreamHandler class"""
    __swig_setmethods__ = {}
    for _s in [Handler]:
        __swig_setmethods__.update(getattr(_s, '__swig_setmethods__', {}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, StreamHandler, name, value)
    __swig_getmethods__ = {}
    for _s in [Handler]:
        __swig_getmethods__.update(getattr(_s, '__swig_getmethods__', {}))
    __getattr__ = lambda self, name: _swig_getattr(self, StreamHandler, name)
    __repr__ = _swig_repr

    def __init__(self, *args):
        """
        __init__(logging::StreamHandler self, LogLevel level) -> StreamHandler
        __init__(logging::StreamHandler self) -> StreamHandler
        __init__(logging::StreamHandler self, io::OutputStream * stream, LogLevel level) -> StreamHandler
        __init__(logging::StreamHandler self, io::OutputStream * stream) -> StreamHandler
        """
        this = _logging.new_StreamHandler(*args)
        try:
            self.this.append(this)
        except:
            self.this = this
    __swig_destroy__ = _logging.delete_StreamHandler
    __del__ = lambda self: None

    def close(self):
        """close(StreamHandler self)"""
        return _logging.StreamHandler_close(self)

StreamHandler_swigregister = _logging.StreamHandler_swigregister
StreamHandler_swigregister(StreamHandler)

class FileHandler(StreamHandler):
    """Proxy of C++ logging::FileHandler class"""
    __swig_setmethods__ = {}
    for _s in [StreamHandler]:
        __swig_setmethods__.update(getattr(_s, '__swig_setmethods__', {}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, FileHandler, name, value)
    __swig_getmethods__ = {}
    for _s in [StreamHandler]:
        __swig_getmethods__.update(getattr(_s, '__swig_getmethods__', {}))
    __getattr__ = lambda self, name: _swig_getattr(self, FileHandler, name)
    __repr__ = _swig_repr

    def __init__(self, *args):
        """
        __init__(logging::FileHandler self, std::string const & fname, LogLevel level, int creationFlags) -> FileHandler
        __init__(logging::FileHandler self, std::string const & fname, LogLevel level) -> FileHandler
        __init__(logging::FileHandler self, std::string const & fname) -> FileHandler
        """
        this = _logging.new_FileHandler(*args)
        try:
            self.this.append(this)
        except:
            self.this = this
    __swig_destroy__ = _logging.delete_FileHandler
    __del__ = lambda self: None
FileHandler_swigregister = _logging.FileHandler_swigregister
FileHandler_swigregister(FileHandler)

class Filter(_object):
    """Proxy of C++ logging::Filter class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Filter, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Filter, name)
    __repr__ = _swig_repr

    def __init__(self, *args):
        """
        __init__(logging::Filter self, std::string name) -> Filter
        __init__(logging::Filter self) -> Filter
        """
        this = _logging.new_Filter(*args)
        try:
            self.this.append(this)
        except:
            self.this = this
    __swig_destroy__ = _logging.delete_Filter
    __del__ = lambda self: None

    def filter(self, record):
        """filter(Filter self, LogRecord const * record) -> bool"""
        return _logging.Filter_filter(self, record)


    def getName(self):
        """getName(Filter self) -> std::string"""
        return _logging.Filter_getName(self)

Filter_swigregister = _logging.Filter_swigregister
Filter_swigregister(Filter)

class Logger(Filterer):
    """Proxy of C++ logging::Logger class"""
    __swig_setmethods__ = {}
    for _s in [Filterer]:
        __swig_setmethods__.update(getattr(_s, '__swig_setmethods__', {}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, Logger, name, value)
    __swig_getmethods__ = {}
    for _s in [Filterer]:
        __swig_getmethods__.update(getattr(_s, '__swig_getmethods__', {}))
    __getattr__ = lambda self, name: _swig_getattr(self, Logger, name)
    __repr__ = _swig_repr

    def __init__(self, *args):
        """
        __init__(logging::Logger self, std::string name) -> Logger
        __init__(logging::Logger self) -> Logger
        """
        this = _logging.new_Logger(*args)
        try:
            self.this.append(this)
        except:
            self.this = this
    __swig_destroy__ = _logging.delete_Logger
    __del__ = lambda self: None

    def log(self, *args):
        """
        log(Logger self, LogLevel level, std::string const & msg)
        log(Logger self, LogLevel level, except::Context const & ctxt)
        log(Logger self, LogLevel level, except::Throwable const & t)
        """
        return _logging.Logger_log(self, *args)


    def debug(self, *args):
        """
        debug(Logger self, std::string const & msg)
        debug(Logger self, except::Context const & ctxt)
        debug(Logger self, except::Throwable const & t)
        """
        return _logging.Logger_debug(self, *args)


    def info(self, *args):
        """
        info(Logger self, std::string const & msg)
        info(Logger self, except::Context const & ctxt)
        info(Logger self, except::Throwable const & t)
        """
        return _logging.Logger_info(self, *args)


    def warn(self, *args):
        """
        warn(Logger self, std::string const & msg)
        warn(Logger self, except::Context const & ctxt)
        warn(Logger self, except::Throwable const & t)
        """
        return _logging.Logger_warn(self, *args)


    def error(self, *args):
        """
        error(Logger self, std::string const & msg)
        error(Logger self, except::Context const & ctxt)
        error(Logger self, except::Throwable const & t)
        """
        return _logging.Logger_error(self, *args)


    def critical(self, *args):
        """
        critical(Logger self, std::string const & msg)
        critical(Logger self, except::Context const & ctxt)
        critical(Logger self, except::Throwable const & t)
        """
        return _logging.Logger_critical(self, *args)


    def addHandler(self, handler, own=False):
        """
        addHandler(Logger self, Handler handler, bool own=False)
        addHandler(Logger self, Handler handler)
        """
        return _logging.Logger_addHandler(self, handler, own)


    def removeHandler(self, handler):
        """removeHandler(Logger self, Handler handler)"""
        return _logging.Logger_removeHandler(self, handler)


    def setLevel(self, level):
        """setLevel(Logger self, LogLevel level)"""
        return _logging.Logger_setLevel(self, level)


    def setName(self, name):
        """setName(Logger self, std::string const & name)"""
        return _logging.Logger_setName(self, name)


    def getName(self):
        """getName(Logger self) -> std::string"""
        return _logging.Logger_getName(self)


    def reset(self):
        """reset(Logger self)"""
        return _logging.Logger_reset(self)

Logger_swigregister = _logging.Logger_swigregister
Logger_swigregister(Logger)

class NullHandler(Handler):
    """Proxy of C++ logging::NullHandler class"""
    __swig_setmethods__ = {}
    for _s in [Handler]:
        __swig_setmethods__.update(getattr(_s, '__swig_setmethods__', {}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, NullHandler, name, value)
    __swig_getmethods__ = {}
    for _s in [Handler]:
        __swig_getmethods__.update(getattr(_s, '__swig_getmethods__', {}))
    __getattr__ = lambda self, name: _swig_getattr(self, NullHandler, name)
    __repr__ = _swig_repr

    def __init__(self, *args):
        """
        __init__(logging::NullHandler self, LogLevel level) -> NullHandler
        __init__(logging::NullHandler self) -> NullHandler
        """
        this = _logging.new_NullHandler(*args)
        try:
            self.this.append(this)
        except:
            self.this = this
    __swig_destroy__ = _logging.delete_NullHandler
    __del__ = lambda self: None
NullHandler_swigregister = _logging.NullHandler_swigregister
NullHandler_swigregister(NullHandler)

class NullLogger(Logger):
    """Proxy of C++ logging::NullLogger class"""
    __swig_setmethods__ = {}
    for _s in [Logger]:
        __swig_setmethods__.update(getattr(_s, '__swig_setmethods__', {}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, NullLogger, name, value)
    __swig_getmethods__ = {}
    for _s in [Logger]:
        __swig_getmethods__.update(getattr(_s, '__swig_getmethods__', {}))
    __getattr__ = lambda self, name: _swig_getattr(self, NullLogger, name)
    __repr__ = _swig_repr

    def __init__(self, *args):
        """
        __init__(logging::NullLogger self, std::string const & name) -> NullLogger
        __init__(logging::NullLogger self) -> NullLogger
        """
        this = _logging.new_NullLogger(*args)
        try:
            self.this.append(this)
        except:
            self.this = this
    __swig_destroy__ = _logging.delete_NullLogger
    __del__ = lambda self: None
NullLogger_swigregister = _logging.NullLogger_swigregister
NullLogger_swigregister(NullLogger)

class LoggerManager(_object):
    """Proxy of C++ logging::LoggerManager class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, LoggerManager, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, LoggerManager, name)
    __repr__ = _swig_repr

    def __init__(self):
        """__init__(logging::LoggerManager self) -> LoggerManager"""
        this = _logging.new_LoggerManager()
        try:
            self.this.append(this)
        except:
            self.this = this

    def getLoggerSharedPtr(self, *args):
        """
        getLoggerSharedPtr(LoggerManager self, std::string const & name) -> mem::SharedPtr< logging::Logger >
        getLoggerSharedPtr(LoggerManager self) -> mem::SharedPtr< logging::Logger >
        """
        return _logging.LoggerManager_getLoggerSharedPtr(self, *args)


    def getLogger(self, *args):
        """
        getLogger(LoggerManager self, std::string const & name) -> Logger
        getLogger(LoggerManager self) -> Logger
        """
        return _logging.LoggerManager_getLogger(self, *args)

    __swig_destroy__ = _logging.delete_LoggerManager
    __del__ = lambda self: None
LoggerManager_swigregister = _logging.LoggerManager_swigregister
LoggerManager_swigregister(LoggerManager)


def debug(*args):
    """
    debug(std::string const & msg)
    debug(except::Context const & ctxt)
    debug(except::Throwable & t)
    """
    return _logging.debug(*args)

def info(*args):
    """
    info(std::string const & msg)
    info(except::Context const & ctxt)
    info(except::Throwable & t)
    """
    return _logging.info(*args)

def warn(*args):
    """
    warn(std::string const & msg)
    warn(except::Context const & ctxt)
    warn(except::Throwable & t)
    """
    return _logging.warn(*args)

def error(*args):
    """
    error(std::string const & msg)
    error(except::Context const & ctxt)
    error(except::Throwable & t)
    """
    return _logging.error(*args)

def critical(*args):
    """
    critical(std::string const & msg)
    critical(except::Context const & ctxt)
    critical(except::Throwable & t)
    """
    return _logging.critical(*args)

def getLogger(*args):
    """getLogger() -> Logger"""
    return _logging.getLogger(*args)

def getLoggerSharedPtr(*args):
    """getLoggerSharedPtr() -> mem::SharedPtr< logging::Logger >"""
    return _logging.getLoggerSharedPtr(*args)
# This file is compatible with both classic and new-style classes.

