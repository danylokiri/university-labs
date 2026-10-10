package com.example.lab1;

import lombok.*;

@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
public class PowerResult {
    private int base;
    private int exponent;
    private long value;
}
