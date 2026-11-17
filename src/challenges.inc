struct Challenge {
    const char* title;
    const char* instruction;
    bool cpp;
    const char* starter;
    const char* reference;
    std::array<const char*,4> inputs;
    std::array<int,4> expected;
};
constexpr Challenge challenges[]={
    {"Sum", "Sum the integers from 1 through n. Example: 5 produces 15.", false,
     "#include <stdio.h>\nint main(void) {\n    int n; if (scanf(\"%d\", &n)!=1) return 1;\n    int result=0;\n    // TODO: implement the exercise.\n    printf(\"%d\\n\",result);\n    return 0;\n}",
     "#include <stdio.h>\nint main(void) {\n    int n; if (scanf(\"%d\", &n)!=1) return 1;\n    int result=0;\n    for(int i=1;i<=n;++i) result+=i;\n    printf(\"%d\\n\",result);\n    return 0;\n}",
     {"0", "1", "5", "20"}, {0, 1, 15, 210}},
    {"Even count", "Count the even integers from 1 through n. Example: 7 produces 3.", false,
     "#include <stdio.h>\nint main(void) {\n    int n; if (scanf(\"%d\", &n)!=1) return 1;\n    int result=0;\n    // TODO: implement the exercise.\n    printf(\"%d\\n\",result);\n    return 0;\n}",
     "#include <stdio.h>\nint main(void) {\n    int n; if (scanf(\"%d\", &n)!=1) return 1;\n    int result=0;\n    for(int i=1;i<=n;++i) if(i%2==0) ++result;\n    printf(\"%d\\n\",result);\n    return 0;\n}",
     {"0", "1", "7", "12"}, {0, 0, 3, 6}},
    {"Array minimum", "Read three integers and print the smallest. Example: 5 2 8 produces 2.", false,
     "#include <stdio.h>\nint main(void) {\n    int a[3];\n    for(int i=0;i<3;++i) if(scanf(\"%d\",&a[i])!=1) return 1;\n    int result=a[0];\n    // TODO: implement the exercise.\n    printf(\"%d\\n\",result);\n    return 0;\n}",
     "#include <stdio.h>\nint main(void) {\n    int a[3];\n    for(int i=0;i<3;++i) if(scanf(\"%d\",&a[i])!=1) return 1;\n    int result=a[0];\n    for(int i=1;i<3;++i) if(a[i]<result) result=a[i];\n    printf(\"%d\\n\",result);\n    return 0;\n}",
     {"5 2 8", "-2 -9 -1", "2 2 2", "0 4 9"}, {2, -9, 2, 0}},
    {"GCD function", "Implement gcd(a,b) for nonnegative inputs. Use remainder until b becomes zero.", false,
     "#include <stdio.h>\nint gcd(int a,int b) { (void)b; return a; }\nint main(void) {\n    int a,b; if(scanf(\"%d%d\",&a,&b)!=2) return 1;\n    int result=gcd(a,b);\n    // TODO: implement the exercise.\n    printf(\"%d\\n\",result);\n    return 0;\n}",
     "#include <stdio.h>\nint gcd(int a,int b) { while(b) { int r=a%b; a=b; b=r; } return a; }\nint main(void) {\n    int a,b; if(scanf(\"%d%d\",&a,&b)!=2) return 1;\n    int result=gcd(a,b);\n    \n    printf(\"%d\\n\",result);\n    return 0;\n}",
     {"12 18", "17 5", "0 9", "42 0"}, {6, 1, 9, 42}},
    {"Pointer update", "Implement increase so it adds 4 to the caller's value through its pointer.", false,
     "#include <stdio.h>\nvoid increase(int* value) { (void)value; }\nint main(void) {\n    int n; if(scanf(\"%d\",&n)!=1) return 1;\n    increase(&n); int result=n;\n    // TODO: implement the exercise.\n    printf(\"%d\\n\",result);\n    return 0;\n}",
     "#include <stdio.h>\nvoid increase(int* value) { *value+=4; }\nint main(void) {\n    int n; if(scanf(\"%d\",&n)!=1) return 1;\n    increase(&n); int result=n;\n    \n    printf(\"%d\\n\",result);\n    return 0;\n}",
     {"0", "-9", "17", "100"}, {4, -5, 21, 104}},
    {"Vector frequency", "Read three integers and a target. Print how often the target occurs in the vector.", true,
     "#include <iostream>\n#include <vector>\nint main() {\n    std::vector<int> values(3);\n    int target;\n    for(int& v:values) if(!(std::cin>>v)) return 1;\n    if(!(std::cin>>target)) return 1;\n    int result=0;\n    // TODO: implement the exercise.\n    std::cout << result << \"\\n\";\n    return 0;\n}",
     "#include <iostream>\n#include <vector>\nint main() {\n    std::vector<int> values(3);\n    int target;\n    for(int& v:values) if(!(std::cin>>v)) return 1;\n    if(!(std::cin>>target)) return 1;\n    int result=0;\n    for(int v:values) if(v==target) ++result;\n    std::cout << result << \"\\n\";\n    return 0;\n}",
     {"1 2 1 1", "4 4 4 4", "0 1 2 3", "-1 2 3 -1"}, {2, 3, 0, 1}},
};
