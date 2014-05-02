The Field Widget Storage API module is a storage engine that can be used by
any field widget that needs to save extra configurations alongside a field.
Developers are able to leverage the modules API so they don't have to worry
about implementing CRUD functionality for their field widget.

### Use Case:

Hypothetically, say you need to collect a class attribute from a user when an
image is uploaded on a given entity. Currently the Image field type doesn't
expose a way to define class attributes. We're going to have to accomplish
this by defining a new field using the Field API, and rebuild the image
uploading capabilities inside that module. Then implement the code for
collecting the class attribute. That seems like a lot of development time just
to allow a user to input a class attribute for an image.
