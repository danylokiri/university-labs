import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

interface zavdannya5_interface {
    int compute(int x, int y);
}

class my_class implements zavdannya5_interface {
    public int compute(int x, int y) {
        return x + y;
    }
}

class profiling_handler implements InvocationHandler {
    private final Object target;

    public profiling_handler(Object target) {
        this.target = target;
    }

    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        long startTime = System.nanoTime();
        Object result = method.invoke(target, args);
        long endTime = System.nanoTime();
        System.out.println("Execution time of " + method.getName() + ": " + (endTime - startTime) + " nanoseconds");
        return result;
    }
}

class tracing_handler implements InvocationHandler {
    private final Object target;

    public tracing_handler(Object target) {
        this.target = target;
    }

    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        System.out.print("Invoked method: " + method.getName() + " with args: ");
        if (args != null) {
            for (Object arg : args) {
                System.out.print(arg + " ");
            }
        }
        Object result = method.invoke(target, args);
        System.out.println("\nResult: " + result);
        return result;
    }
}

public class zavdannya5 {
    public static void main(String[] args) {
        my_class myClass = new my_class();

        zavdannya5_interface profilingProxy = (zavdannya5_interface) Proxy.newProxyInstance(
                my_class.class.getClassLoader(),
                new Class[] { zavdannya5_interface.class },
                new profiling_handler(myClass));

        zavdannya5_interface tracingProxy = (zavdannya5_interface) Proxy.newProxyInstance(
                my_class.class.getClassLoader(),
                new Class[] { zavdannya5_interface.class },
                new tracing_handler(myClass));

        System.out.println("Profiling proxy:");
        profilingProxy.compute(5, 3);

        System.out.println("\nTracing proxy:");
        tracingProxy.compute(5, 3);
    }
}
