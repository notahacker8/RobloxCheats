
//So we don't forget.

/*
{

    NSURLSession *session = [NSURLSession sharedSession];
    
    NSURLSessionDataTask *dataTask = [session dataTaskWithRequest:request completionHandler:^(NSData *data, NSURLResponse *response, NSError *error)
    {
      NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *)response;
      if(httpResponse.statusCode == 200)
      {
      }
      else
      {
        NSLog(@"Error sending request to %@", request.URL);
      }
    }];
    [dataTask resume];
}
*/
