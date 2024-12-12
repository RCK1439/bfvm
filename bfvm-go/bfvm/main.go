package main

import (
	"fmt"
	"log"
	"os"
)

func main() {
	log.SetPrefix("bfvm: ")
	log.SetFlags(0)
	args := os.Args

	if len(args) < 2 {
		log.Fatal("no sources")
	}

	filepath := args[1]
	fmt.Printf("Filepath: %s\n", filepath)
}
