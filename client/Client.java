package inject.client;

public class Client {

    // Main native methods
    public static native byte[] GetClassBytes(Class<?> clazz);
    public static native int SetClassBytes(Class<?> clazz, byte[] classBytes);
    public static native String[] GetClassFields(Class<?> clazz, boolean fieldNames);
    public static native String[] GetClassMethods(Class<?> clazz, boolean methodNames);

    // JNI native methods
    public static native String GetClassMapped(String seargeClassName);
    public static native Class<?> FindClass(String seargeClassName);
    public static native String GetMethodMapped(String seargeClassName, String seargeMethodName, String seargeMethodDescriptor);
    public static native String GetFieldMapped(String seargeClassName, String seargeFieldName);

    public static void test(int times) {
        for (int i = 0; i < times; ++i) {
            System.out.println("Hello world " + i);
        }

        String className = "net/minecraft/client/Minecraft";
        Class<?> mcClass = FindClass(className);
        String mappedClass = GetClassMapped(className);
        System.out.println(className + " -> " + mappedClass);

        byte[] classBytes = GetClassBytes(mcClass);
        System.out.println("Class bytes length: " + classBytes.length);

        int result = SetClassBytes(mcClass, classBytes);
        System.out.println("Set class bytes");

        System.out.println("Class fields: ");
        String[] fieldNames = GetClassFields(mcClass, true);
        String[] fieldDescriptors = GetClassFields(mcClass, false);
        for (int i = 0; i < fieldNames.length; ++i) {
            String fieldName = fieldNames[i];
            String fieldDescriptor = fieldDescriptors[i];
            System.out.print(fieldName + " " + fieldDescriptor + (i == fieldNames.length - 1 ? "" : ", "));
        }
        System.out.println();

        System.out.println("Class methods: ");
        String[] methodNames = GetClassMethods(mcClass, true);
        String[] methodDescriptors = GetClassMethods(mcClass, false);
        for (int i = 0; i < methodNames.length; ++i) {
            String methodName = methodNames[i];
            String methodDescriptor = methodDescriptors[i];
            System.out.print(methodName + " " + methodDescriptor + (i == methodNames.length - 1 ? "" : ", "));
        }
        System.out.println();

        String fieldName = "theMinecraft";
        String mappedField = GetFieldMapped(className, fieldName);
        System.out.println(fieldName + " -> " + mappedField);

        String methodName = "getMinecraft";
        String methodDescriptor = "()Lnet/minecraft/client/Minecraft;";
        String mappedMethod = GetMethodMapped(className, methodName, methodDescriptor);
        System.out.println(methodName + " " + methodDescriptor + " -> " + mappedMethod);
    }
}
