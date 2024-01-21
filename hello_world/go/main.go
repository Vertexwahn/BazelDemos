package main

/*
 *  SPDX-FileCopyrightText: 2022-2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

import "fmt"
import "runtime"

func main() {
	fmt.Println("Hello World from Go!")
	fmt.Printf("Go version: %s\n", runtime.Version())
}
