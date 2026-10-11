package com.example.lab1;

import org.springframework.stereotype.Component;

@Component
public class PowerOperation {

    public long power(int a, int b) {
        long result = 1;

        if (b < 0) {
            throw new IllegalArgumentException("b must be >=0");
        }

        for (int i = 0; i < b; i++) {
            result *= a;
        }

        return result;
    }
}
