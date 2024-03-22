import Image from "next/image";

type Props = {
  title: string;
  src: string;
};

const AuthorImage = ({ title, src }: Props) => {
  return (
    <div>
      <Image
        src={src}
        alt={`Image for ${title}`}
        width={1300} // Specify your desired width
        height={630} // Specify your desired height
        className="w-full" // Adjust if you have specific styling in mind
      />
    </div>
  );
};

export default AuthorImage;
