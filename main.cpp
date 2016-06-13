#include <boost/python.hpp>

namespace bp = boost::python;

bp::object import(const std::string& module, const std::string& path)
{
    bp::object globals = bp::import("__main__").attr("__dict__");

    bp::dict locals;
    locals["module_name"] = module;
    locals["path"]        = path;

    bp::exec("import imp\n"
             "new_module = imp.load_module(module_name, open(path), path, ('bp', 'U', imp.PY_SOURCE))\n",
             globals,
             locals);
    return locals["new_module"];
}

///////////////////////////

class Runner
{
public:
    void init(bp::object script)
    {
        // capture methods at creation time so we don't have to look them up every time we call them
        _run    = script.attr("run");
        _result = script.attr("result");
    }

    void run()
    {
        _run(); // call the script's run method
    }

    void execute(int i)
    {
        result(i * 2);
    }

private:
    void result(int i)
    {
        _result(i); // call the script's result method
    }

    bp::object _run;
    bp::object _result;
};

int main()
{
    Py_Initialize();

    // load our python script and extract the Script class
    bp::object module = import("test", "test.py");
    bp::object Script = module.attr("Script");

    // wrap Runner and expose some functions to python
    bp::object RunnerWrapper(
        bp::class_<Runner>("Runner")
            .def("execute", &Runner::execute)
        );

    // create a python wrapped instance of Runner, which we will pass to the script so it can call back through it
    bp::object wrapper = RunnerWrapper();
    bp::object script  = Script(wrapper);

    // extract the runner instance from the python wrapped instance
    Runner& runner = bp::extract<Runner&>(wrapper);

    // initialise with the script, so we can get handles to the script's methods we require
    runner.init(script);

    runner.run();

    Py_Finalize();
    return 0;
}
