
/*
 So we don't forget.
 */




#pragma mark - Making a request -




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




#pragma mark - Dispatch Loop -





/*

dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
               {
    for (;;)
    {
    }
});
 
*/



