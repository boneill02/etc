/*
   this program prompts the user for ingredients they own, and tells them
   what they can cook with it.
 */
package main

import (
	"fmt"
	"strings"
)

var ingredients []string
var recipes []Recipe

type Recipe struct {
	name string
	ingredients []string
}

func main() {
	var response string
	var ownedIngredients []string
	canCook := false

	initializeVariables()

	/* finds out what ingredients the user owns */
	for _, ingredient := range ingredients {
		fmt.Printf("Do you have %s? (y/n): ", ingredient)
		fmt.Scanf("%s\n", &response)
		if strings.ToLower(response) == "yes" || strings.ToLower(response) == "y" {
			ownedIngredients = append(ownedIngredients, ingredient)
		}
	}

	/* check if any known recipes can be made using ingredients the user possesses */
	for _, recipe := range recipes {
		canMakeThis := true

		for _, rs := range recipe.ingredients {
			if !contains(ownedIngredients, rs) {
				canMakeThis = false
			}
		}

		/* all required ingredients are available */
		if canMakeThis {
			canCook = true
			fmt.Printf("You can make %s.\n", recipe.name)
		}
	}

	/* the user cannot make anything that we know of with what they have */
	if !canCook {
		fmt.Printf("I don't know what you can make with those items.\n")
	}
}

/* store ingredients and recipes */
func initializeVariables() {
	ingredients = []string {
		"flour",
		"yeast",
		"sugar",
		"salt",
		"eggs",
		"parmesan cheese",
		"mozzarella cheese",
		"milk",
		"potatoes",
		"butter",
		"oregano",
	}

	recipes = []Recipe {
		Recipe{"Bread", []string { "flour", "yeast", "sugar", "salt" } },
		Recipe{"Gnocchi", []string { "flour", "potatoes", "butter", "oregano" } },
	}
}

/* check if an item is in a given integer array */
func contains(haystack []string, needle string) bool {
	for _, i := range haystack {
		if needle == i {
			return true
		}
	}
	return false
}
