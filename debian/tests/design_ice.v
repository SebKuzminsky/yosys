module design_ice(input ck, input I1, output O1);

    reg ready = 0;
    reg value;

    always @(posedge ck) begin
        if(ready) begin
            value   <= I1;
        end
        else begin
            ready   <= 1;
        end

    end


    assign O1 = value;

endmodule
