#define CATCH_CONFIG_MAIN
#include<catch.hpp>




SCENARIO("Hello World") {
	GIVEN("the string hello world") {
		auto helloWorld = "HelloWorld";

		THEN("strcmp should return 0") {
			auto retVal = strcmp("HelloWorld", helloWorld);
			REQUIRE(retVal == 0);
		}
	}

}