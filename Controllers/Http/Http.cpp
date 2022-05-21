//
// Created by Alberto Romero on 20/4/22.
//

#include "Http.h"

//std::unique_ptr<RestClient> Http::client = restc_cpp::RestClient::Create();

Http::Http(std::string uri, HttpMethod method, std::map<std::string, std::string> queryParams, std::string optionalJsonPayload) : uri(uri) {

    if (this->Created && false)
    {
        std::cout << "Http Client was already created. Call WebRequest method" << std::endl << std::endl;
        return;
    }

    this->uri = uri;
    this->method = method;
    this->optionalJsonPayload = optionalJsonPayload;
    this->queryParams = queryParams;

    this->Created = true;
}

std::string Http::WebRequest() {

    if (!this->Created)
    {
        std::cout << "Http Client is not created. Call constructor first." << std::endl << std::endl;
        return "error";
    }


    // Response that will be returned in the future<T>
    std::string finalResponse;



    //Http* http = this;


    auto clientr = restc_cpp::RestClient::Create();
    // En lambda
    auto done = clientr->ProcessWithPromise([&](Context& ctx) {

        std::unique_ptr<Reply> cachedResponse;
        RequestBuilder&& builder(ctx); // creo builder

        // Método
        (method == HttpMethod::Post ? builder.Post(uri) : builder.Get(uri));

        if (!optionalJsonPayload.empty())
        {
           // RequestBody body;
            builder.Data(optionalJsonPayload);
            builder.AddHeader("Content-Type", "application/json");
        }

        //builder.Data()

        // Main headers
        builder.AddHeader("Accept", "application/json");

        // Add query parameters
        if (!this->queryParams.empty())
        {
            for (std::pair<const std::basic_string<char>, std::basic_string<char>> &element : this->queryParams)
            {
                //std::cout << "Elements: " << element.first << element.second;
                builder.Argument(element.first, element.second);
            }
        }

        // Execute the request
        cachedResponse = builder.Execute();
        finalResponse = cachedResponse->GetBodyAsString();

    });

    try {

        done.get();

    } catch(const std::exception& ex) {
        std::cout << "Main thread: Http controller exception: "
             << ex.what() << std::endl;
    }

    return finalResponse; // return it
}
